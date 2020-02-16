#include <vector>
#include "common.h"
using namespace std;

class Mesh {
private:
    void apply_force(particle_t& particle, particle_t& neighbor);
    void apply_force_single_cell(particle_t* particle, int r, int c);
    int get_cellRow(particle_t p);
    int get_cellColumn(particle_t p);
public:
    int nRow, nColumn;

    double cellSize;
    vector<particle_t*>* cells;
    void initialize(int row, int column, double cell_size);
    void generate_mesh(particle_t* parts, int num_parts);
    bool Insert(particle_t * p, int r, int c) {
        cells[r * nRow + c].push_back(p);

    }
    bool Clear() {
        for (int i = 0; i < nRow; i++) {
            for (int j = 0; j < nColumn; j++) {
                cells[i * nRow + j].clear();
            }
        }
    }
    void apply_force_neighbor_cell(int r, int c);
    void apply_force_mesh();
};

