# Copyright (C) 2014 Colin Bernet
# https://github.com/cbernet/heppy/blob/master/LICENSE

from ROOT import TFile

class Events(object):
    '''Event list from a tree in a root file.
    '''
    def __init__(self, filename, treename):
        self.file = TFile(filename)
        if self.file.IsZombie():
            raise ValueError('file {fnam} does not exist'.format(fnam=filename))
        self.tree = self.file.Get(treename)
        if self.tree == None: # is None would not work
            raise ValueError('tree {tree} does not exist in file {fnam}'.format(
                tree = treename,
                fnam = filename
                ))

    def size(self):
        return self.tree.GetEntries()

    def to(self, iEv):
        '''navigate to event iEv.'''
        self.tree.GetEntry(iEv)
        return self.tree

    def __iter__(self):
        return iter(self.tree)

if __name__ == '__main__':

    import sys
    events = Events(sys.argv[1], 'test_tree')
    print '\naccessing one event directly'
    event = events.to(2)
    print event.var1
    print '\nlooping on all events:'
    for iev, ev in enumerate(events):
         if iev==9:
             print '...'
         if iev>=9 and iev<990:
             continue
         else:
             print ev.var1
