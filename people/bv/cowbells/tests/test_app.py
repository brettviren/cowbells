#!/usr/bin/env python

import cowbells

def test_app():
    cowbells.mcapp.InitMC("NullConfig.C")
    print 'MC name: "%s"' % cowbells.mc.GetName()
    cowbells.app._geant4.ProcessGeantMacro("g4config2.in")
    #cowbells.app._geant4.ProcessGeantMacro("g4vis.in")
    cowbells.mcapp.RunMC(10)
if __name__ == '__main__':
    test_app()

