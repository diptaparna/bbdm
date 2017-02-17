#! /usr/bin/env python
import sys,os,subprocess

def usage():
    print ' - Usage: ./getmaps.py benchmark_point cross section'
    print ' - Example: ./getmaps.py T2tt_650_50.txt -1'
    print ' - Only the last run of a given analysis is taken'
    print ' - If a negative cross section is given, a cross section of'
    print '   0.001 fb is used and no information on the best region is'
    print '   displayed in the output'

def GetAnalysisNumber(name):
  rpos = name.rfind('_')
  nr= int(name[rpos+1:])
  aname=name[:rpos]
  if nr>Numbers[aname]:
    Numbers[aname]=nr

# at least one argument, check if asking for help
if len(sys.argv) < 3 or (sys.argv[1] == "-h" or sys.argv[1] == "--help"):
    usage()
    sys.exit()

# xsection information
if float(sys.argv[2]) < 0:
   xsec = 0.001
else:
   xsec = float(sys.argv[2])

# get the list of the relevant analysis
analysispath = os.getcwd()+'/Output/'+sys.argv[1]
dirlist=[dirnames[0] for dirnames in os.walk(analysispath)]
dirlist = [ dir for dir in dirlist if not('Cutflows' in dir or 'Histograms' in dir or dir==analysispath)]
dirlist = [ dir.replace(analysispath,'') for dir in dirlist]
dirlist = [ dir.replace('/','',1) for dir in dirlist if dir[0]=='/']
anames = [ dir[:dir.rfind('_')] for dir in dirlist]
anames = list(set(anames))
Numbers = dict([analysis,-1] for analysis in anames)
for dir in dirlist:
  GetAnalysisNumber(dir)
for analysis in anames:
  TheCommand = './exclusion_CLs.py '+analysis+' '+sys.argv[1]+' ' +str(Numbers[analysis])+' ' + str(xsec)
  try:
    result=subprocess.Popen([TheCommand],shell=True,stdout=subprocess.PIPE,cwd=os.getcwd())
  except:
    print 'ERROR: impossible to execute the command: ' + TheCommand+'\n'
    sys.exit()
  ok, err = result.communicate()
  if not ok:
    print "ERROR with the extraction of the efficiencies. Check the ", analysis, "analysis...\n"

