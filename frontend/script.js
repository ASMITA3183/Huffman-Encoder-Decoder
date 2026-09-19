// =========================================
// GET HTML ELEMENTS
// =========================================

// Compression elements

const compressDropZone = document.getElementById("compressDropZone");

const compressFileInput = document.getElementById("compressFileInput");

const compressBrowseButton = document.getElementById("compressBrowseButton");

const compressSelectedFile = document.getElementById("compressSelectedFile");

const compressButton = document.getElementById("compressButton");

// Decompression elements

const decompressDropZone = document.getElementById("decompressDropZone");

const decompressFileInput = document.getElementById("decompressFileInput");

const decompressBrowseButton = document.getElementById(
  "decompressBrowseButton"
);

const decompressSelectedFile = document.getElementById(
  "decompressSelectedFile"
);

const decompressButton = document.getElementById("decompressButton");

// Status elements

const statusIndicator = document.getElementById("statusIndicator");

const statusText = document.getElementById("statusText");

// =========================================
// STORE SELECTED FILES
// =========================================

let fileToCompress = null;

let fileToDecompress = null;

// =========================================
// STATUS FUNCTION
// =========================================

function updateStatus(message, type = "normal") {
  statusText.textContent = message;

  if (type === "success") {
    statusIndicator.style.background = "#4ade80";
  } else if (type === "error") {
    statusIndicator.style.background = "#f87171";
  } else if (type === "working") {
    statusIndicator.style.background = "#facc15";
  } else {
    statusIndicator.style.background = "#555b64";
  }
}

// =========================================
// COMPRESS FILE SELECTION
// =========================================

compressBrowseButton.addEventListener("click", () => {
  compressFileInput.click();
});

compressFileInput.addEventListener("change", () => {
  const file = compressFileInput.files[0];

  if (file) {
    selectFileForCompression(file);
  }
});

// =========================================
// HANDLE COMPRESS FILE
// =========================================

function selectFileForCompression(file) {
  fileToCompress = file;

  compressSelectedFile.textContent = `Selected: ${file.name}`;

  compressButton.disabled = false;

  updateStatus(`Ready to compress "${file.name}".`);
}

// =========================================
// DECOMPRESS FILE SELECTION
// =========================================

decompressBrowseButton.addEventListener("click", () => {
  decompressFileInput.click();
});

decompressFileInput.addEventListener("change", () => {
  const file = decompressFileInput.files[0];

  if (file) {
    selectFileForDecompression(file);
  }
});

// =========================================
// HANDLE DECOMPRESS FILE
// =========================================

function selectFileForDecompression(file) {
  const filename = file.name.toLowerCase();

  if (!filename.endsWith(".huff")) {
    fileToDecompress = null;

    decompressSelectedFile.textContent = "Please select a .huff file.";

    decompressButton.disabled = true;

    updateStatus("Decompression requires a .huff file.", "error");

    return;
  }

  fileToDecompress = file;

  decompressSelectedFile.textContent = `Selected: ${file.name}`;

  decompressButton.disabled = false;

  updateStatus(`Ready to decompress "${file.name}".`);
}

// =========================================
// DRAG & DROP - COMPRESS
// =========================================

compressDropZone.addEventListener("dragover", (event) => {
  event.preventDefault();

  compressDropZone.classList.add("dragover");
});

compressDropZone.addEventListener("dragleave", () => {
  compressDropZone.classList.remove("dragover");
});

compressDropZone.addEventListener("drop", (event) => {
  event.preventDefault();

  compressDropZone.classList.remove("dragover");

  const file = event.dataTransfer.files[0];

  if (file) {
    selectFileForCompression(file);
  }
});

// =========================================
// DRAG & DROP - DECOMPRESS
// =========================================

decompressDropZone.addEventListener("dragover", (event) => {
  event.preventDefault();

  decompressDropZone.classList.add("dragover");
});

decompressDropZone.addEventListener("dragleave", () => {
  decompressDropZone.classList.remove("dragover");
});

decompressDropZone.addEventListener("drop", (event) => {
  event.preventDefault();

  decompressDropZone.classList.remove("dragover");

  const file = event.dataTransfer.files[0];

  if (file) {
    selectFileForDecompression(file);
  }
});

// =========================================
// COMPRESS BUTTON
// =========================================

compressButton.addEventListener("click", async () => {
  if (!fileToCompress) {
    updateStatus("Please select a file first.", "error");

    return;
  }

  try {
    updateStatus(`Compressing "${fileToCompress.name}"...`, "working");

    compressButton.disabled = true;

    // Create form data
    const formData = new FormData();

    formData.append("file", fileToCompress);

    // Send file to C++ server
    const response = await fetch("/compress", {
      method: "POST",
      body: formData,
    });

    // Check whether compression succeeded
    if (!response.ok) {
      const errorMessage = await response.text();

      throw new Error(errorMessage);
    }

    // Get compressed .huff file
    const blob = await response.blob();

    // Create a temporary download URL
    const downloadUrl = URL.createObjectURL(blob);

    // Create download link
    const downloadLink = document.createElement("a");

    downloadLink.href = downloadUrl;

    downloadLink.download = `${fileToCompress.name}.huff`;

    // Start download
    document.body.appendChild(downloadLink);

    downloadLink.click();

    // Remove temporary link
    downloadLink.remove();

    // Release temporary URL
    URL.revokeObjectURL(downloadUrl);

    updateStatus(
      `Compression completed. "${fileToCompress.name}.huff" downloaded.`,
      "success"
    );
  } catch (error) {
    console.error("Compression error:", error);

    updateStatus(`Compression failed: ${error.message}`, "error");
  } finally {
    compressButton.disabled = false;
  }
});

// =========================================
// DECOMPRESS BUTTON
// =========================================

decompressButton.addEventListener("click", async () => {
  if (!fileToDecompress) {
    updateStatus("Please select a .huff file first.", "error");

    return;
  }

  try {
    updateStatus(`Decompressing "${fileToDecompress.name}"...`, "working");

    decompressButton.disabled = true;

    const formData = new FormData();

    formData.append("file", fileToDecompress);

    const response = await fetch("/decompress", {
      method: "POST",
      body: formData,
    });

    if (!response.ok) {
      const errorMessage = await response.text();

      throw new Error(errorMessage);
    }

    const blob = await response.blob();

    const downloadUrl = URL.createObjectURL(blob);

    const downloadLink = document.createElement("a");

    downloadLink.href = downloadUrl;

    /*
     * Remove .huff from the filename.
     *
     * Example:
     * test.txt.huff
     * becomes:
     * test.txt
     */
    const originalFilename = fileToDecompress.name.endsWith(".huff")
      ? fileToDecompress.name.slice(0, -5)
      : "restored_file";

    downloadLink.download = originalFilename;

    document.body.appendChild(downloadLink);

    downloadLink.click();

    downloadLink.remove();

    URL.revokeObjectURL(downloadUrl);

    updateStatus(
      `Decompression completed. "${originalFilename}" downloaded.`,
      "success"
    );
  } catch (error) {
    console.error("Decompression error:", error);

    updateStatus(`Decompression failed: ${error.message}`, "error");
  } finally {
    decompressButton.disabled = false;
  }
});
