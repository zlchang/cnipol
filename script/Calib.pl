#! /usr/bin/perl
# Last modfied: Feb.16,2006
##############  defulats ##############
$CALIBDIR     = $ENV{"CALIBDIR"};
$OPT_GHOSTVIEW=" ";
$PUBLISH=0;
#######################################


#----------------------------------------------------------------------
#               Command Line Options
#----------------------------------------------------------------------
use Getopt::Std;
my %opt;
getopts('f:hpg', \%opt);

if ( $opt{h} ) {
    help();
}
if ( $opt{p} ) {
    $PUBLISH=1;
}
if ( $opt{g} ) {
    $OPT_GHOSTVIEW="-g";
}

# Get Run ID
my $Runn = $opt{f};
if (length ($Runn) == 0){
    print "Error: Specify calibration <filename>.\n";
    help();
}

sub help(){
    print "\n";
    print " Usage:\n  $0 -hpg [ -f <filename>]\n\n"; 
    print "    Create energy calibration histograms and execute fit. \n";
    print "    CalibGen.pl & CalibCal.pl \n\n";
    print "\t -f <filename> calibration file name w/o .data \n";
    print "\t -p            Publish calibration data file to $CALIBDIR\n";
    print "\t -g            Execute ghostscript to view  fitted results.\n";
    print "\t -h            Show this help";
    print "\n\n";
    print "    ex.) Calib.pl -f bluc_calib_0504\n";
    print "\n";
    exit(0);
}


######################################################################
#                   PUBLISH CALIBRATION FILE                         #
######################################################################
if ($PUBLISH) {Publish()};

sub Publish(){
    $CALIBDATA= "calib/$Runn.temp.dat";
    if (-e $CALIBDATA){
	system("install --mode 666 -v -C $CALIBDATA $CALIBDIR");
    }else{
	printf("ERROR: $CALIBDATA doesn't exist. Make calibration file first.\n");
	exit(-1);
    }
    exit(0);
}




#########################################################################
######                     Main Routine                           #######
#########################################################################

printf("CALIB DATA : $Runn \n");
system("echo 'Generating histograms...\n'");
system("CalibGen.pl -f $Runn \n");
system("echo 'Executing Fitting...\n'");
system("CalibCal.pl -f $Runn $OPT_GHOSTVIEW \n");


