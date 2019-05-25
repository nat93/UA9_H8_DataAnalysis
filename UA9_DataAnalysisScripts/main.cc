///////////////////////////////////////////////////////////////////////
///                Date: 25/05/2019                                 ///
///               Autor: Andrii Natochii                            ///
/// Program description: It illustrates how to analyze data for the ///
///                      channeling efficiency measuremenst and     ///
///                      inelastic nuclear interaction              ///
///                      investigation at the SPS H8 extraction     ///
///                      beam line with proton, pion, and ion beams.///
///////////////////////////////////////////////////////////////////////

#include "src/data_ana.hh"
#include "src/includes.hh"

using namespace std;

int main(int argc, char *argv[])
{
    cout<<endl<<endl;
    if(argc != 5)
    {
        cout<<endl;
        cout<<"--> ERROR:: Wrong number of input parameters!"<<endl;
        cout<<"--> [0]: script name"<<endl;
        cout<<"--> [1]: intput file name"<<endl;
        cout<<"--> [2]: run mode: 0 - histograms; 1 - torsion/efficiency/optimization; 2 - ini fixed; 3 - ini angular scan"<<endl;
        cout<<"--> [3]: year: >= 2015"<<endl;
        cout<<"--> [4]: runID: 0 - CH (with angular cut); 1 - AM/BKG (without angular cut)"<<endl;
        cout<<endl;
        assert(0);
    }
    else
    {
        cout<<endl;
        printf("--> script name \t [0]: %s\n",argv[0]);
        printf("--> intput file name \t [1]: %s\n",argv[1]);
        printf("--> run mode \t\t [2]: %s (0 - histograms; 1 - torsion/efficiency/optimization; 2 - ini fixed; 3 - ini angular scan)\n",argv[2]);
        printf("--> year \t\t [3]: %s (>= 2015)\n",argv[3]);
        printf("--> runID \t\t [4]: %s (0 - CH (with angular cut); 1 - AM/BKG (without angular cut))\n",argv[4]);
        cout<<endl;
    }

    time_t start_time, stop_time;
    start_time = time(NULL);

    string filenamepath     = argv[1];
    size_t pos              = filenamepath.find_last_of('/');
    string filename         = filenamepath.substr(pos+1);
    string output_dir       = "./output/";
    TString output_filename;

    data_ana *pointer = new data_ana(filenamepath.data());

    switch (atoi(argv[2]))
    {
    case 0:
        // HISTOGRAMS
        pointer->plot_histogramms();

        output_filename     = output_dir;
        output_filename     += filename;
        output_filename     += "_HISTOGRAMS.root";

        pointer->write_histo_to_file(output_filename.Data());
        break;
    case 1:
        // TORSION/EFFICIENCY/OPTIMIZATION
        output_filename     = output_dir;
        output_filename     += filename;
        output_filename     += "_TORSION_EFFICIENCY_OPTIMIZATION.root";

        pointer->get_torsion_efficiency_optimization(output_filename.Data());
        break;
    case 2:
        // INI FIXED GONIO POSITION
        output_filename     = output_dir;
        output_filename     += filename;
        output_filename     += "_FIXED_COUNTS.dat";

        pointer->find_inelastic_counts_fixed(output_filename.Data(),atof(argv[3]),atoi(argv[4]));
        break;
    case 3:
        // INI ANGULAR SCAN
        output_filename     = output_dir;
        output_filename     += filename;
        output_filename     += "_SCAN_COUNTS.dat";

        pointer->find_inelastic_counts_scan(output_filename.Data(),atof(argv[3]),atoi(argv[4]));
        break;
    default:
        cout<<endl<<">>>>> WARNING: Nothing to do for the given run mode ! <<<<<"<<endl<<endl;
        break;
    }

    pointer->~data_ana();

    stop_time = time(NULL);
    cout<<"--> Running time is : "<<stop_time - start_time<<" seconds"<<endl;
    return 0;
}
