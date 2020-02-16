#include "common.h"
#include <cmath>
#include "mesh.h"
using namespace std;

Mesh mesh;

// Integrate the ODE
void move(particle_t& p, double size) {
    // Slightly simplified Velocity Verlet integration
    // Conserves energy better than explicit Euler method
    p.vx += p.ax * dt;
    p.vy += p.ay * dt;
    p.x += p.vx * dt;
    p.y += p.vy * dt;

    // Bounce from walls
    while (p.x < 0 || p.x > size) {
        p.x = p.x < 0 ? -p.x : 2 * size - p.x;
        p.vx = -p.vx;
    }

    while (p.y < 0 || p.y > size) {
        p.y = p.y < 0 ? -p.y : 2 * size - p.y;
        p.vy = -p.vy;
    }
}


void init_simulation(particle_t* parts, int num_parts, double size) {
	// You can use this space to initialize static, global data objects
    // that you may need. This function will be called once before the
    // algorithm begins. Do not do any particle simulation here
    int row, column;
    row = column = ceil(size/(2 * cutoff));
    mesh.initialize(row, column, size/row);
    mesh.generate_mesh(parts, num_parts);
}

void simulate_one_step(particle_t* parts, int num_parts, double size) {
    // Compute Forces
    mesh.apply_force_mesh();

    // Move Particles
    for (int i = 0; i < num_parts; ++i) {
        move(parts[i], size);
    }
    mesh.Clear();
    mesh.generate_mesh(parts, num_parts);
}
