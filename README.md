# bbdm
MadAnalysis recasting code for CMS-PAS-B2G-15-007

# How to Use
First download MadAnalysis5 v1.3 and extract it into convenient rocation.

Then cd into the madanalysis5 directory and run ./bin/ma5, it will perform all the initial checks.

Use the following commands in MadAnalysis interactive shell -

install zlib

install fastjet

install delphes

If root is not configured globally, also execute this command -

install root

Once done, exit and run Madanalysis in reco mode by typing ./bin/ma5 -R, it will check if all the components are installed correctly. Now type -

install RecastingTools

Once completed, exit and run Madanalysis in expert mode with reco support by typing ./bin/ma5 -R -E. Choose "DarkMatter" as folder name and "cms_pp_bbdm" as analysis name. Then cd into the "DarkMatter/Build" folder and replace the "SampleAnalyser" dir with the one given here.

Next, type "source setup.sh" in sh/bash or "source setup.csh" in csh/tcsh. This will set up all the required environment variables. Finally, type "make" to build make the "MadAnalysis5Job" executable.

In the "Input" dir, there is a file "input.txt". Put the full path of the delphes output file (.root) in that file.

Now, inside "Build" dir, type -
./MadAnalysis5Job ../Input/input.txt

This will make the MadAnalysis5Job to run on that particular delphes(.root) file.
