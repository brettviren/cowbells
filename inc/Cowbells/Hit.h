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

	// Production type, subtype.  Some possibilities:
	// 2, 2 eIoni
	// 2, 2 hIoni
	// 2, 3 eBrem
	// 2, 12 phot
	// 2, 13 compt
	// 2, 21 Cerenkov
	// 2, 22 Scintillation
	// 3, 34 OpWLS
	// 4, 111 hadElastic
	int pType();
	void setpType(int t);

	int pSubType();
	void setpSubType(int st);

    private:
        double t, x, y, z, e;	// time, global position and energy
        int volid, hcid;	// Volume and hit collection IDs
	int pdg, ptype, psubtype; // Parent PDG ID and production mechanism type/subtype
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
