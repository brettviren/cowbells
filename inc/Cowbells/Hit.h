/**
 * \class Cowbells::Hit
 *
 * \brief Big in Japan.
 *
 * A cowbellshit bag of data from a hit on a Cowbells::SensitiveDetector.
 *
 * bv@bnl.gov Fri May 11 15:37:31 2012
 *
 */


#ifndef COWBELLS_HIT_H
#define COWBELLS_HIT_H

#include <G4THitsCollection.hh>
#include <G4VHit.hh>

#include <vector>

namespace Cowbells {

    enum ProductionCode {
	unknown = 0,
	ceren,			// Cherenkov
	scint,			// Scintillation (directly from edep)
	reem,			// Light absorption/re-emission
    };

    class Hit {
    public:
        Hit();
        ~Hit();
        // Time of hit relative to primary vertex time
        double time() const;
        void setTime(double t);

        // Global hit position
        std::vector<double> pos();
        void setPos(double x, double y, double z);

        // Logical volume copy number 
        int volId();
        void setVolId(int id);

        // Hit Collection ID
        int hcId();
        void setHcId(int id);

       // PDG particle id of the parent that produced the optical photon
        int pdgId();
        void setPdgId(int pid);

        // Energy deposition
        double energy();
        void setEnergy(double e);

	// ProductionCode saying how the photon was produced
	int pCode();
	void setpCode(int code);

    private:
        double t, x, y, z, e;	// time, global position and energy
        int volid, hcid;	// Volume and hit collection IDs
	int pdg, pcode;	   // Parent PDG ID and production mechanism code
    };

    class GHit : public G4VHit 
    {
    public:
        GHit(Cowbells::Hit* hit) {
            m_hit = hit;
        }
        virtual ~GHit() {
            if (!m_hit) { return; }
            delete m_hit; 
            m_hit = 0;
        }
        Cowbells::Hit* get() { return m_hit; }
    private:
        Cowbells::Hit* m_hit;
    };

    typedef G4THitsCollection<Cowbells::GHit> HitCollection;

} // namespace Cowbells

#endif  // COWBELLS_HIT_H
