#include <vector>
#include "common.h"
using namespace std;

class Mesh {
private:
    void apply_force(particle_t& particle, particle_t& neighbor);
    inline void apply_force_single_cell(particle_t* particle, int r, int c);
    inline int get_cellRow(particle_t p);
    inline int get_cellColumn(particle_t p);
public:
    int nRow, nColumn;

    double cellSize;
    vector<particle_t*>* cells;
    void initialize(int row, int column, double cell_size);
    inline void generate_mesh(particle_t* parts, int num_parts);
    inline void Insert(particle_t * p, int r, int c) {
        cells[r * nRow + c].push_back(p);
    }
    inline void Clear() {
        for (int i = 0; i < nRow; i++) {
            for (int j = 0; j < nColumn; j++) {
                cells[i * nRow + j].clear();
            }
        }
    }
    void apply_force_neighbor_cell(int r, int c);
    inline void apply_force_mesh();
};

