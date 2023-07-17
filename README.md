

---

# ezTrim

**ezTrim** is a parallel quality trimming tool designed to improve the quality and efficiency of your sequencing data processing.

## 🌟 Features

- Quality Trimming
- Adapter Trimming
- Compressed Output

## 🚀 Getting Started

### Prerequisites

Ensure that you have the following installed on your system:
- g++
- OpenMP
- zlib

### Installation

Compile the program using the following command:

```bash
cd program/parallelP/
g++ -std=c++2a main.cpp -o ezTrim -fopenmp -lz
```

## 📖 Usage

```bash
ezTrim -i <input_file> -p <pattern> [-t <temp_path>] [-o <out_file_name>] [-h <output_path>] [-w <window_size>] [-b <base_size>] [-t <trim_threshold>] [-v <3_to_5_prime_trim>]
```

### Parameters

#### Required

| Parameter   | Default | Description                               |
|-------------|---------|-------------------------------------------|
| `-i`        | None    | Input file (e.g., file.fastq.gz)          |
| `-p`        | None    | Adapter pattern to be removed             |

#### Optional

| Parameter   | Default            | Description                               |
|-------------|--------------------|-------------------------------------------|
| `-t`        | Current directory  | Path to store temporary files             |
| `-o`        | Current directory  | Name for the output file                  |
| `-h`        | Current directory  | Path to store output files                |
| `-w`        | 4                  | Alternative window size                     |
| `-b`        | 50                 | Alternative base size                       |
| `-t`        | 20                 | Alternative trim threshold                  |
| `-v`        | Forward            | Option for 3 to 5 prime end trimming      |

---

