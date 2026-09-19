#include "httplib.h"

#include "encoder/HuffmanEncoder.h"
#include "decoder/HuffmanDecoder.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

// Get only the filename and remove any directory path.
std::string getSafeFilename(const std::string &filename)
{

    fs::path path(filename);

    std::string safeName =
        path.filename().string();

    if (safeName.empty())
    {
        return "uploaded_file";
    }

    return safeName;
}

int main()
{

    httplib::Server server;

    // =========================================
    // SERVE FRONTEND
    // =========================================

    if (!server.set_mount_point(
            "/",
            "../frontend"))
    {

        std::cerr
            << "Error: Could not mount frontend folder.\n";

        return 1;
    }

    // =========================================
    // COMPRESS FILE
    // =========================================

    server.Post(
        "/compress",
        [](const httplib::Request &request,
           httplib::Response &response)
        {
            // Check whether a file was uploaded.
            if (!request.form.has_file("file"))
            {

                response.status = 400;

                response.set_content(
                    "No file uploaded.",
                    "text/plain");

                return;
            }

            const auto &uploadedFile =
                request.form.get_file("file");

            // Get a safe filename.
            std::string filename =
                getSafeFilename(
                    uploadedFile.filename);

            // Create input/output directories
            // if they don't already exist.
            fs::create_directories("input");
            fs::create_directories("output");

            std::string inputPath =
                "input/" + filename;

            std::string outputPath =
                "output/" + filename + ".huff";

            // =========================================
            // SAVE UPLOADED FILE
            // =========================================

            std::ofstream inputFile(
                inputPath,
                std::ios::binary);

            if (!inputFile)
            {

                response.status = 500;

                response.set_content(
                    "Unable to save uploaded file.",
                    "text/plain");

                return;
            }

            inputFile.write(
                uploadedFile.content.data(),
                static_cast<std::streamsize>(
                    uploadedFile.content.size()));

            inputFile.close();

            // =========================================
            // COMPRESS USING OUR C++ CODE
            // =========================================

            try
            {

                HuffmanEncoder encoder;

                encoder.compress(
                    inputPath,
                    outputPath);
            }
            catch (const std::exception &e)
            {

                response.status = 500;

                response.set_content(
                    std::string(
                        "Compression failed: ") +
                        e.what(),
                    "text/plain");

                fs::remove(inputPath);

                return;
            }

            // =========================================
            // READ COMPRESSED FILE
            // =========================================

            std::ifstream outputFile(
                outputPath,
                std::ios::binary | std::ios::ate);

            if (!outputFile)
            {

                response.status = 500;

                response.set_content(
                    "Unable to read compressed file.",
                    "text/plain");

                fs::remove(inputPath);

                return;
            }

            std::streamsize fileSize =
                outputFile.tellg();

            outputFile.seekg(
                0,
                std::ios::beg);

            std::string compressedData(
                static_cast<size_t>(fileSize),
                '\0');

            if (fileSize > 0)
            {

                outputFile.read(
                    compressedData.data(),
                    fileSize);
            }

            outputFile.close();

            // =========================================
            // SEND .HUFF FILE TO BROWSER
            // =========================================

            response.set_header(
                "Content-Disposition",
                "attachment; filename=\"" +
                    filename +
                    ".huff\"");

            response.set_content(
                compressedData,
                "application/octet-stream");

            // =========================================
            // CLEAN TEMPORARY FILES
            // =========================================

            fs::remove(inputPath);
            fs::remove(outputPath);
        });

    // =========================================
    // DECOMPRESS FILE
    // =========================================

    server.Post(
        "/decompress",
        [](const httplib::Request &request,
           httplib::Response &response)
        {
            // Check whether a file was uploaded.
            if (!request.form.has_file("file"))
            {

                response.status = 400;

                response.set_content(
                    "No file uploaded.",
                    "text/plain");

                return;
            }

            const auto &uploadedFile =
                request.form.get_file("file");

            // Get a safe filename.
            std::string filename =
                getSafeFilename(
                    uploadedFile.filename);

            // Make sure the uploaded file is .huff.
            if (
                filename.length() < 5 ||
                filename.substr(
                    filename.length() - 5) != ".huff")
            {

                response.status = 400;

                response.set_content(
                    "Only .huff files can be decompressed.",
                    "text/plain");

                return;
            }

            // Create input/output directories.
            fs::create_directories("input");
            fs::create_directories("output");

            std::string inputPath =
                "input/" + filename;

            // Remove .huff extension.
            std::string outputFilename =
                filename.substr(
                    0,
                    filename.length() - 5);

            std::string outputPath =
                "output/" + outputFilename;

            // =========================================
            // SAVE UPLOADED .HUFF FILE
            // =========================================

            std::ofstream inputFile(
                inputPath,
                std::ios::binary);

            if (!inputFile)
            {

                response.status = 500;

                response.set_content(
                    "Unable to save uploaded file.",
                    "text/plain");

                return;
            }

            inputFile.write(
                uploadedFile.content.data(),
                static_cast<std::streamsize>(
                    uploadedFile.content.size()));

            inputFile.close();

            // =========================================
            // DECOMPRESS USING OUR C++ CODE
            // =========================================

            try
            {

                HuffmanDecoder decoder;

                decoder.decompress(
                    inputPath,
                    outputPath);
            }
            catch (const std::exception &e)
            {

                response.status = 500;

                response.set_content(
                    std::string(
                        "Decompression failed: ") +
                        e.what(),
                    "text/plain");

                fs::remove(inputPath);

                return;
            }

            // =========================================
            // READ RESTORED FILE
            // =========================================

            std::ifstream outputFile(
                outputPath,
                std::ios::binary | std::ios::ate);

            if (!outputFile)
            {

                response.status = 500;

                response.set_content(
                    "Unable to read decompressed file.",
                    "text/plain");

                fs::remove(inputPath);

                return;
            }

            std::streamsize fileSize =
                outputFile.tellg();

            outputFile.seekg(
                0,
                std::ios::beg);

            std::string restoredData(
                static_cast<size_t>(fileSize),
                '\0');

            if (fileSize > 0)
            {

                outputFile.read(
                    restoredData.data(),
                    fileSize);
            }

            outputFile.close();

            // =========================================
            // SEND RESTORED FILE TO BROWSER
            // =========================================

            response.set_header(
                "Content-Disposition",
                "attachment; filename=\"" +
                    outputFilename +
                    "\"");

            response.set_content(
                restoredData,
                "application/octet-stream");

            // =========================================
            // CLEAN TEMPORARY FILES
            // =========================================

            fs::remove(inputPath);
            fs::remove(outputPath);
        });
    // =========================================
    // START SERVER
    // =========================================

    std::cout
        << "Huffman server started at "
        << "http://localhost:8080\n";

    std::cout
        << "Serving frontend from ../frontend\n";

    std::cout
        << "Compression endpoint: POST /compress\n";

    const char *portEnv = std::getenv("PORT");
    int port = portEnv ? std::stoi(portEnv) : 8080;

    server.listen("0.0.0.0", port);

    return 0;
}