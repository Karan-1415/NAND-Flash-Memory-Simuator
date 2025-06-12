# NAND-Flash-Memory-Simuator
# NAND Flash Memory Simulator with Dynamic Wear Leveling

![C](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Embedded](https://img.shields.io/badge/Domain-Embedded_Systems-orange?style=for-the-badge)

---

## Overview

This project simulates a **NAND Flash Memory** system with a focus on **dynamic wear leveling** — a technique used to extend the lifespan of flash memory by evenly distributing write/erase cycles across all blocks.

The simulator models the core behavior of NAND flash:
- Block and page management  
- Wear counting and thresholds  
- Logical-to-Physical (L2P) address mapping  
- Dynamic wear leveling to balance wear across blocks  

---

## Features

- **Wear Leveling Algorithm**  
  Distributes write/erase operations to prevent premature wear-out of specific blocks.

- **Page Status Management**  
  Tracks page states: free, valid, or invalid.

- **Logical to Physical Address Translation**  
  Maintains address mapping for efficient data access.

- **Block Erase and Wear Counter**  
  Simulates block erase operations and keeps track of block wear levels.

---

## Technologies Used

- **Language:** C  
- **Concepts:** Memory management, wear leveling algorithms, flash memory simulation, embedded system fundamentals.

---

## How It Works

1. **Memory Initialization**  
   Initializes blocks, pages, and wear counters with default states.

2. **Data Writing & Mapping**  
   Finds the least worn block with free pages, writes data, and updates logical-to-physical mappings.

3. **Dynamic Wear Leveling**  
   Periodically moves valid data from highly worn ("hot") blocks to less worn ("cold") blocks, balancing wear.

4. **Block Erase**  
   Erases blocks after data migration, incrementing wear counters.

---

## Usage

- The main simulation runs in `main()`:
  - Initializes memory.  
  - Finds the best block to write data.  
  - Writes to a free page and updates mapping.  
  - Performs wear leveling to balance usage.

---

## Example Output

L2P[42] = Block 15, Page 3

---

## Future Work

- Implement garbage collection for invalid pages.  
- Simulate more realistic error handling and bad block management.  
- Extend simulator to support multi-level cell (MLC) NAND.  

---

## Author

**Suraj Karan**  
Aspiring Embedded Systems Engineer  
[LinkedIn](https://www.linkedin.com/in/suraj-karan-a52846227/)

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

