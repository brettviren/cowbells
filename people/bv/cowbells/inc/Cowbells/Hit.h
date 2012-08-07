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
        double time() const { return m_time; }
        void setTime(double t) { m_time = t; }

        // Global hit position
        std::vector<double> pos();
        void setPos(double x, double y, double z);

        // Logical volume copy number 
        int volId() { return m_volid; }
        void setVolId(int id) {m_volid = id;}

        // Energy deposition
        double energy() {return m_energy;}
        void setEnergy(double e) {m_energy = e;}

    private:
        double m_time, m_x, m_y, m_z, m_energy;
        int m_volid;
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
