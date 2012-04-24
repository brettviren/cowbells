#!/usr/bin/env python

import cowbells

def test_app():
    cowbells.mcapp.SetVerboseLevel(9)

    macroname = "g4config.in"
    print 'Processing macro "%s"' % macroname
    cowbells.g4vmc.ProcessGeantMacro(macroname)

    print 'Initializing MC: "%s"' % cowbells.mc.GetName()
    cowbells.mcapp.InitMC("NullConfig.C")

    print 'Running 10'
    cowbells.mcapp.RunMC(10)
    print 'And, I am out'
    return

if __name__ == '__main__':
    test_app()

