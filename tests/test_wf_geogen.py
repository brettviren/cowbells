#!/usr/bin/env python


from cowbells.workflow import geogen

from btdtwf import workflow, process

p = process.register('magic_gen', geogen.callable, geogen.input_parameters, geogen.output_parameters)
wf = workflow.Workflow()
wf.add(p)
print wf(provenance_name='test_wf_geogen', geogen_builder='aquarium')

