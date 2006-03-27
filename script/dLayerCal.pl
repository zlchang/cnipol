#! /usr/bin/perl
# defualts 
$GHOSTVIEW=0;
# HID:15000 (const.t) HID:15100: (banana)
$HID  = 15000; 
# Fit Energy Rnage Default
$EMIN=400;
$EMAX=900;
$cfile="config.dat";
$ONLINE_CONFIG=" ";

#----------------------------------------------------------------------
#               Command Line Options
#----------------------------------------------------------------------
use Getopt::Std;
my %opt;
getopts('E:f:bgh', \%opt);

if ( $opt{h} ) {
    help();
}
if ( $opt{g} ) {
    $GHOSTVIEW=1;
}
if ( $opt{b} ){
    $HID=15100;
}

# Get Run ID
my $Runn = $opt{f};
if (length ($Runn) == 0){
    print "Error: Specify <runID>.\n";
    help();
}


# Get Fit Energy Range
if ( $opt{E} ){
    @field = split(/:/,$opt{E});
    if (length($field[0]) != 0){ $EMIN=$field[0];}
    if (length($field[1]) != 0){ $EMAX=$field[1];}
}


sub help(){
    print "\n";
    print " Usage:\n  $0 -hgb [ -f <runID>] [-E <Emin:Emax>]\n\n"; 
    print "    fit banana histograms and get deadlayer and t0.\n";
    print "    Execute dLayerGen.pl for histogram creation.\n\n";
    print "\t -f <runID>     runID\n";
    print "\t -b             Fit on banana cut events. (def:const.t cut)\n";
    print "\t -g             Launch ghostviewer after fit.\n";
    print "\t -E <Emin:Emax> Fit Energy Range in [keV] (def <$EMIN:$EMAX>) \n";
    print "\t -h             Show this help \n";
    print "\n";
    print "    ex.1) dLayerCal.pl -E 350:950 -f 7279.005 -b \n\n";
    print "\n";
    exit(0);
}

sub GhostView(){
    system("gv dlayer/$Runn.fittemp.ps");
    system("gv dlayer/$Runn.residual.ps");
    system("gv dlayer/$Runn.summarytemp.ps");
}



#----------------------------------------------------------------------
#               Directory Path Setup
#----------------------------------------------------------------------
$BASEDIR      = $ENV{"ASYMDIR"};
$MACRODIR     = $ENV{"MACRODIR"};
$DLAYERDIR    = "$BASEDIR/dlayer";
$CONFDIR      = $ENV{"CONFDIR"};

unless (-d $DLAYERDIR) {
    mkdir $DLAYERDIR;
}


#----------------------------------------------------------------------
#         Get Online Configulation File Name from Online Log
#----------------------------------------------------------------------
sub GetOnlineConfig(){

$ONLINEDIR    = $ENV{"ONLINEDIR"};
$ONLINELOG    = "$ONLINEDIR/log/$Runn.log";
unless (open(ONLINE_LOG_FILE,"$ONLINELOG")) {
    # This is not a cruecial for offline analysis. Should be recovered later.
    die "Error: $ONLINELOG doesn't exist. \n";
}
while (<ONLINE_LOG_FILE>) {
    if (/Reading calibration parameters from file/) {
	($F1,$F2,$F3,$F4,$F5,$F6,$F7,$F8) = split;
	$ONLINE_CONFIG="$CONFDIR/$F8";
    };
};

}

#----------------------------------------------------------------------
#           Get Run Condistions from Offline Log
#----------------------------------------------------------------------
sub GetLog(){

unless (open(LOGFILE,"douts/$Runn.dl.log")) {
    die "Error: douts/$Runn.dl.log doesn't exist. Run dLayerGen.pl first.\n";
}

while (<LOGFILE>) {
    if (/MASSCUT/) {
	($F1,$F2,$F3) = split;
	$MASSCUT=$F3;
    } elsif (/Beam Energy/) {
	($F1,$F2,$F3,$F4) = split;
	$Bene=$F4;
    } elsif (/RHIC Beam/) {
	($F1,$F2,$F3,$F4) = split;
	$RHICBeam=$F4;
    } elsif (/Kinematic Const. E2T/) {
	($F1,$F2,$F3,$F4,$F5) = split;
	$E2T=$F5;
    } elsif (/CONFIG/) {
	($F1,$F2,$F3) = split;
	$cfile=$F3;
    };
};

}


#----------------------------------------------------------------------
#               Print Run Condition
#----------------------------------------------------------------------

sub PrintRunCondition(){

    printf("RUN NUMBER  : $Runn \n");
    printf("Mass Cut    : $MASSCUT \n");
    printf("Beam Energy : $Bene \n");
    printf("RHICBeam    : $RHICBeam \n");
    printf("E2T         : $E2T \n");
    printf("Emin - Emax : $EMIN - $EMAX \n");
    printf("HID         : $HID \n");
    printf("Config File   : $cfile \n");
    printf("Online Config : $ONLINE_CONFIG \n");

}

#----------------------------------------------------------------------
#                             Main Routine
#----------------------------------------------------------------------

# Get Run Conditions from Offline Log file
GetLog();
# Get Online Configulation file name for online monitoring.
GetOnlineConfig();
# Print Run Conditions
PrintRunCondition();

# Make input macro for fitting.
system("echo '.x $MACRODIR/ExeKinFit.C(\"$Runn\", $Bene, $RHICBeam, $E2T, $EMIN, $EMAX, $HID,\"$cfile\",\"$ONLINE_CONFIG\")' > input.C");
# Execute deadlayer fitting on root
system("root -b < input.C");
    
if (-f "testfit.dat")    {system("mv testfit.dat $DLAYERDIR/$Runn.temp.dat");}
if (-f "testfit.ps")     {system("mv testfit.ps $DLAYERDIR/$Runn.fittemp.ps");}
if (-f "testsummary.ps") {system("mv testsummary.ps $DLAYERDIR/$Runn.summarytemp.ps");}
if (-f "residual.ps")    {system("mv residual.ps $DLAYERDIR/$Runn.residual.ps");}
if (-f "input.C")        {system("rm input.C");}

if ($GHOSTVIEW) {GhostView();};





