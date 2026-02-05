# CPU-Cache-Design-and-Performance-Simulation
This project analyzes the CPU-Cache interaction, a core computer architecture component, based on William Stallings' theoretical frameworks. It provides a simulation environment to measure software efficiency at the hardware level, focusing on mapping techniques and performance optimization.

**Note:** The project simulation and technical presentation are prepared in Turkish, as this was developed as part of my Software Engineering coursework at the Faculty of Engineering.

## 🚀 Key Features (Öne Çıkan Özellikler)
* **Mathematical Modeling:** Implementation of William Stallings' Average Memory Access Time (AMAT) formula.
* **Mapping Analysis:** Comparative study of Direct, Fully Associative, and Set-Associative mapping techniques.
* **Performance Metrics:** Real-time calculation of Hit/Miss rates and their impact on CPU latency.
* **Simulation Environment:** Developed on **Xubuntu Linux** to analyze hardware-software interaction.

## 📊 Methodology and Visuals
Below are key slides from the technical presentation explaining the system architecture and simulation logic.

**Architecture Visualisation**: The digital logic designs and architectural schematics of this project are prepared using Logisim. To view and interact with the .circ files, please ensure you have Logisim installed on your system.

*Figure 1: System architecture and cache mapping logic.*

**Full-Associative Mapping**

<img width="1920" height="1080" alt="Full-associative" src="https://github.com/user-attachments/assets/56ad4ca0-942d-4552-a79c-a4c774422209" />

**Set-Associative**

<img width="1920" height="1080" alt="2-way associative" src="https://github.com/user-attachments/assets/c628ad99-3634-4d0d-94ee-cfcc5b334a59" />

**Direct Mapping**

<img width="1920" height="1080" alt="Direct" src="https://github.com/user-attachments/assets/d5c0df82-c0fc-46cc-974d-ae82f4fb8599" />


## 🛠️ How to Run & Analyze

### 1. Technical Analysis (Teknik Analiz)
For a detailed theoretical background and performance evaluation, please review the files in the `/Documents-Belgeler` folder:
* `cache_mimari_analizi_sunum.pptx`: Comprehensive presentation of the project.
* `ÖNBELLEK EŞLEME TEKNİKLERİ PERFORMANS ANALİZİ`: Detailed system analysis report.
* `Sistem Analizi Ve Tasarımı`: Systems Analysis and Design

### 2. Running the Simulation (Simülasyonu Çalıştırma)
The simulation is developed in **C**. To run it on your local machine (preferably Linux/Xubuntu):

1. Clone the repository:
   ```bash
   git clone https://github.com/Tarek-Talab/CPU-Cache-Design-and-Performance-Simulation.git
