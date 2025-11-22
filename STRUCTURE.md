# Repository Structure

```
CSCD58/
│
├── README.md                           # Main repository overview
├── .gitignore                          # Git ignore rules
│
├── assignments/                        # Course assignments
│   ├── assignment1/
│   │   ├── README.md                  # Assignment 1 overview
│   │   └── D58_Assignment1.pdf        # Assignment specification
│   ├── assignment2/
│   │   ├── README.md
│   │   └── D58_Assignment2.pdf
│   ├── assignment3/
│   │   ├── README.md
│   │   └── D58_Assignment3.pdf
│   └── assignment4/
│       ├── README.md
│       └── D58_Assignment4.pdf
│
├── labs/                               # Laboratory exercises
│   ├── lab03-mininet-basics/
│   │   ├── README.md
│   │   └── LabReport03.pdf
│   ├── lab05-routing-and-switching/
│   │   ├── README.md
│   │   └── lab05_SR.pdf
│   ├── lab07-network-simulators/
│   │   ├── README.md
│   │   ├── D58-lab07.pdf
│   │   └── LabReport07.pdf
│   └── lab09-simple-router/
│       ├── README.md
│       └── D58_Lab09.pdf
│
├── simple-router/                      # Software router project
│   ├── README.md                      # Detailed project documentation
│   ├── src/                           # Router source code (add your files here)
│   └── docs/                          # Additional documentation
│
├── ns3/                               # NS3 simulator work
│   ├── build-instructions.md          # Comprehensive NS3 setup guide
│   └── example-scripts/               # Simulation scripts (add your .cc files here)
│
└── resources/                         # Course materials
    ├── README.md
    ├── slides/                        # Tutorial slides (add here)
    └── reference-material/            # Additional references
```

## Quick Navigation

- **Main README**: [`README.md`](./README.md)
- **Simple Router**: [`simple-router/README.md`](./simple-router/README.md)
- **NS3 Setup**: [`ns3/build-instructions.md`](./ns3/build-instructions.md)

## What to Add Next

### For Simple Router Project:
1. Add your router source code to `simple-router/src/`
2. Add configuration files: `rtable`, `config.sh`, `run_mininet.sh`, `run_pox.sh`
3. Update `simple-router/docs/` with design documents

### For NS3 Simulations:
1. Add your `.cc` simulation scripts to `ns3/example-scripts/`
2. Add any custom topology files
3. Add analysis results or graphs

### For Resources:
1. Add tutorial slides to `resources/slides/`
2. Add reference materials to `resources/reference-material/`

---

**Repository Status:** ✅ Structure Complete | 📝 Ready for Code

