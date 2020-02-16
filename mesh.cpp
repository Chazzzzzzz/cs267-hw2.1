#include "mesh.h"
#include <vector>
#include <algorithm>
#include "common.h"
#include <cmath>
using namespace std;

void Mesh::apply_force(particle_t& particle, particle_t& neighbor) {
    // Calculate Distance
    double dx = neighbor.x - particle.x;
    double dy = neighbor.y - particle.y;
    double r2 = dx * dx + dy * dy;

    // Check if the two particles should interact
    if (r2 > cutoff * cutoff)
        return;

    r2 = fmax(r2, min_r * min_r);
    double r = sqrt(r2);

    // Very simple short-range repulsive force
    double coef = (1 - cutoff / r) / r2 / mass;
    particle.ax += coef * dx;
    particle.ay += coef * dy;
}

void Mesh::initialize(int row, int column, double cell_size) {
    nRow = row;
    nColumn = column;
    cellSize = cell_size;
    cells = new vector<particle_t*>[row * column];
}

int Mesh::get_cellRow(particle_t p) {
    int x;
    x = floor(p.x / cellSize);
    if (x == nRow) {
        x--;
    }
    return x;
}

int Mesh::get_cellColumn(particle_t p){
    int y;
    y = floor(p.y / cellSize);
    if (y == nColumn) {
        y--;
    }
    return y;
}

void Mesh::generate_mesh(particle_t *parts, int num_parts) {
    for(int i = 0; i< num_parts; i++) {
        int x = get_cellRow(parts[i]);
        int y = get_cellColumn(parts[i]);
        this->Insert(&parts[i], x, y);
    }
}

void Mesh::apply_force_single_cell(particle_t* particle, int r, int c) {
    // check legal coordinate
    if (r < 0 or r >= nRow or c < 0 or c >= nColumn) {
        return;
    }
    vector<particle_t*> particles = cells[r * nRow + c];
    for (vector<particle_t*>::iterator it = particles.begin() ; it != particles.end(); ++it) {
        this->apply_force(*particle, **it);
    }
}

void Mesh::apply_force_neighbor_cell(int r, int c) {
    vector<particle_t*> particles = cells[r * nRow + c];
    for (std::vector<particle_t*>::iterator it = particles.begin() ; it != particles.end(); ++it) {
        // up
        apply_force_single_cell(*it, r-1, c);
        // down
        apply_force_single_cell(*it, r+1, c);
        // left
        apply_force_single_cell(*it, r, c-1);
        // right
        apply_force_single_cell(*it, r, c+1);
        // up left
        apply_force_single_cell(*it, r-1, c-1);
        // up right
        apply_force_single_cell(*it, r-1, c+1);
        // down left
        apply_force_single_cell(*it, r+1, c-1);
        // down right
        apply_force_single_cell(*it, r+1, c+1);
    }
}

void Mesh::apply_force_mesh() {
    for (int i = 0; i < nRow; i++) {
        for (int j = 0; j < nColumn; j++) {
            apply_force_neighbor_cell(i, j);
        }
    }
}