#!/usr/bin/env python


from cowbells.workflow import geogen, detsim

from btdtwf import workflow, process

wf = workflow.Workflow()

# the tasks
gen = process.register('aqua_gen', geogen.callable, geogen.input_parameters, geogen.output_parameters)
sim = process.register('aqua_sim', detsim.callable, detsim.input_parameters, detsim.output_parameters)

# defaults
gen.inputs.geogen_builder = 'aquarium'
gen.inputs.geogen_params = 'Gap=Acrylic'
sim.inputs.nevents=3

# hook up dependencies
sim.inputs.geofile = gen.outputs.out_geofile

wf.add(gen)
res = wf(provenance_name='test_wf_detsim')

print res


