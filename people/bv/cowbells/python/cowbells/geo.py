#!/usr/bin/env python
'''
Operate on a geometry file.
'''

import ROOT

def node_path(top, name):
    '''
    Return all paths to named named volume
    '''
    pvname = top.GetName()
    myname = pvname[:pvname.rfind('_')]
    print myname

    if myname == name:          # I'm it
        print 'Found me: "%s"' % myname
        return [[myname]]

    ret = []
    for ind in range(top.GetNdaughters()):
        child = top.GetDaughter(ind)
        got_em = node_path(child, name)
        if not got_em: continue
        for one in got_em:
            ret.append([pvname] + one)
        continue
    return ret

def format_node_path(np):
    '''
    Convert node path in form of list of lists to std::vector<std::string>
    '''
    path = ROOT.std.vector("std::string")()
    ROOT.SetOwnership(path,0)
    path.push_back("")          # reserve element 0.
    for p in np:
        print p
        sp = '%'.join(p[1:])
        sp = '&' + sp
        string = ROOT.std.string(sp)
        path.push_back(string)
    return path
        
def touchable_paths(top, name):
    '''
    Return a vector<string> of touchable paths
    '''
    np = node_path(top,name)
    return format_node_path(np)

def dump(top, depth = 0):
    tab = ' '*depth
    lv = top.GetVolume()
    med = top.GetMedium()
    if med: matname = med.GetName()
    else: matname = "None"
    print '%spv=%s, lv=%s, med=%s' % (tab, top.GetName(), lv.GetName(), matname)
    for ind in range(top.GetNdaughters()):
        child = top.GetDaughter(ind)
        dump(child, depth + 1)
    return


if __name__ == '__main__':
    import sys
    geo = ROOT.TGeoManager.Import(sys.argv[1])
    top = geo.GetTopNode()
    dump(top)
    npath = node_path(top, 'PC')
    for path in npath:
        print path
    vpath = format_node_path(npath)
    for ind in range(vpath.size()):
        print vpath[ind]
