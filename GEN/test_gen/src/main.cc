#include <iostream>
#include <cstring>
#include "ParticleDataFileW.hh"

using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "USAGE: " << argv[0] << " <outfile.root>" << endl;
        return 2;
    }

    string output_fn = argv[1];

    ParticleDataFileW particleDataFile;
    if (!particleDataFile.open(output_fn.c_str())) {
        cout << "root file open failed: " << output_fn << endl;
        return 1;
    }

    int numberOfParticles = 100;
    for (int i = 0; i < numberOfParticles; i++) {
        strcpy(particleDataFile.t_particle.ParticleName, "gamma");
        particleDataFile.t_particle.KinEnergy = 80.0;
        particleDataFile.t_particle.EmitTime = i;
        particleDataFile.t_particle.EmitPosition[0] = 2.0;
        particleDataFile.t_particle.EmitPosition[1] = 2.0;
        particleDataFile.t_particle.EmitPosition[2] = 300.0;
        particleDataFile.t_particle.MomDirection[0] = 0.0;
        particleDataFile.t_particle.MomDirection[1] = 0.0;
        particleDataFile.t_particle.MomDirection[2] = -1.0;
        particleDataFile.t_particle.Polarization[0] = 1.0;
        particleDataFile.t_particle.Polarization[1] = 0.0;
        particleDataFile.t_particle.Polarization[2] = 0.0;
        particleDataFile.fill_entry();

    }

    particleDataFile.write_tree();
    particleDataFile.close();

    return 0;
}
