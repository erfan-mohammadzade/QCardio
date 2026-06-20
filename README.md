# QCardio
Qt+Cardio (C++ Open source project for review ecg signal and export it)
## Overview
This project provides a validation framework for ECG processing libraries. The core requirement is that any ECG software must produce correct outputs when tested against physician-annotated signals.
To support this, we have converted the MIT-BIH Arrhythmia Database into a structured, easy-to-use format suitable for automated testing and validation.

## Dataset Conversion

The MIT-BIH dataset has been restructured to:

- **Standardize signal representation** – Uniform sampling rates, formats, and metadata
- **Preserve physician annotations** – Beat labels, rhythm changes, and signal quality markers
- **Enable automated testing** – Clear separation of input signals and expected outputs
- **Support multiple formats** – CSV, NumPy arrays, and WFDB-compatible structures
- 
### Key Features
- Plot : Plotting ECG Data of dataset
- Conversion : Export to Sample Vector by data conversion
  This project is under develope
