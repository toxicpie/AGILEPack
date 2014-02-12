#include "Base"
#include "cmd-parser/include/parser.hh"
#include <time.h>

void config_info();

void complain(const std::string &complaint);

const std::string timestamp(void);


//----------------------------------------------------------------------------
int main(int argc, char const *argv[])
{
    std::string s("A simple CLI for AGILEPack centered around providing ");
    s += "functionality for \ntraining and testing Deep Learning";

    optionparser::parser p(s + " models on ROOT TTrees");

//----------------------------------------------------------------------------
    p.add_option("--file", "-f")    .help("Pass at least one file to add to a TChain for training.")
                                    .mode(optionparser::store_mult_values);
//----------------------------------------------------------------------------
    p.add_option("--tree", "-t")    .help("Name of the TTree to extract.")
                                    .mode(optionparser::store_value);
//----------------------------------------------------------------------------
    p.add_option("--save", "-s")    .help("Name of file to save the YAML neural network file to.")
                                    .mode(optionparser::store_value)
                                    .default_value(std::string("neural_net" + timestamp() + ".yaml"));
//----------------------------------------------------------------------------
    p.add_option("--learning")      .help("Pass a learning rate.")
                                    .mode(optionparser::store_value)
                                    .default_value(0.1);
//----------------------------------------------------------------------------
    p.add_option("--momentum")      .help("Pass a momentum value.")
                                    .mode(optionparser::store_value)
                                    .default_value(0.5);
//----------------------------------------------------------------------------
    p.add_option("--regularize")    .help("Pass an l2 norm regularizer.")
                                    .mode(optionparser::store_value)
                                    .default_value(0.00001);
    p.add_option("--batch")         .help("Mini-batch size.")
                                    .mode(optionparser::store_value)
                                    .default_value(10);
//----------------------------------------------------------------------------
    p.add_option("--load")          .help("Name of a YAML neural network file to load to begin training")
                                    .mode(optionparser::store_value);
//----------------------------------------------------------------------------
    std::string config_help = "Pass a configuration file for training specifications instead\n";
    config_help.append(25, ' ');
    config_help += "of over the command line. For help on formatting, pass\n";
    config_help.append(25, ' ');
    config_help += "the '-confighelp' flag. This overrides all paramaters from CLI.\n";
    config_help.append(25, ' ');
    config_help += "This is required for specifying training variables.\n";

    p.add_option("--config", "-c")  .help(config_help)
                                    .mode(optionparser::store_value);
//----------------------------------------------------------------------------
    p.add_option("-confighelp")     .help("Display info about YAML training config files.");
//----------------------------------------------------------------------------
    std::string struct_help = "Pass the structure of the neural network. For example, one\n";
    struct_help.append(25, ' ');
    struct_help += "could specify --struct=13 24 23 15 3 [other flags].";

    p.add_option("--struct")        .help(struct_help)
                                    .mode(optionparser::store_mult_values);
//----------------------------------------------------------------------------
    std::string autoencoder_help = "Pretraining using deep autoencoders, if you pass an\n";
    autoencoder_help.append(25, ' ');
    autoencoder_help += "integer n, it will train the first n layers. It defaults to all.";

    p.add_option("--deepauto", "-d").help(autoencoder_help)
                                    .mode(optionparser::store_value)
                                    .default_value(-1);
//----------------------------------------------------------------------------
    std::string type_help = "Specify the type of predicive target we are trying to \n";
    type_help.append(25, ' ');
    type_help += "work with. Can be one of 'regress', 'multiclass', or 'binary'.";

    p.add_option("--type", "-T")    .help(type_help)
                                    .mode(optionparser::store_value)
                                    .default_value("regress");
//----------------------------------------------------------------------------
    p.add_option("--verbose", "-v") .help("Make the output verbose");
//----------------------------------------------------------------------------
    p.add_option("-start")          .help("Start index for training. (Default = 0)")
                                    .mode(optionparser::store_value)
                                    .default_value(0);
//----------------------------------------------------------------------------
    p.add_option("-end")            .help("End index for training. (Default, whole tree)")
                                    .mode(optionparser::store_value)
                                    .default_value(-1);  
//----------------------------------------------------------------------------
    p.add_option("-epochs")         .help("Number of passes over the Trees. (Default = 10)")
                                    .mode(optionparser::store_value)
                                    .default_value(10);
//----------------------------------------------------------------------------
    std::string prog_string = "Output progress files every (n) epochs. If n isn't passed,\n";
    prog_string.append(25, ' ');
    p.add_option("-prog")           .help(prog_string + "uses default. (Default = 1)")
                                    .mode(optionparser::store_value)
                                    .default_value(1);
//----------------------------------------------------------------------------
    p.eat_arguments(argc, argv);

    if (p.get_value("confighelp")) config_info();

    if (!p.get_value("file")) complain("need to pass at least one file.");

    if (!p.get_value("tree")) complain("need to pass a tree name.");

    // if (!p.get_value("config")) complain("need a config file for variable specification.");

    if (!p.get_value("struct")) complain("need to pass a struct");



    std::vector<std::string> root_files(p.get_value<std::vector<std::string>>("file"));

    std::string ttree_name =  p.get_value<std::string>("tree"),
                // config_file = p.get_value<std::string>("config"),
                save_file =   p.get_value<std::string>("save");


    double  learning =    p.get_value<double>("learning"), 
            momentum =    p.get_value<double>("momentum"),
            regularizer = p.get_value<double>("regularize");


    int     deepauto =    p.get_value<int>("deepauto"),
            start =       p.get_value<int>("start"),
            end =         p.get_value<int>("end"),
            epochs =      p.get_value<int>("epochs"),
            batch =       p.get_value<int>("batch"),
            prog =        p.get_value<int>("prog");

    bool    verbose =     p.get_value("verbose");

    std::vector<int> structure = p.get_value<std::vector<int>>("struct");


    std::cout << "all parameters in!" << std::endl;


//----------------------------------------------------------------------------


    agile::root::tree_reader TR;


    for (auto &file : root_files)
    {
       TR.add_file(file, ttree_name);
    }

    std::cout << "added files!" << std::endl;
    TR.set_branch("pt", agile::root::double_precision);
    TR.set_branch("bottom", agile::root::integer);
    TR.set_branch("charm", agile::root::integer);
    TR.set_branch("light", agile::root::integer);
    TR.set_branch("eta", agile::root::double_precision);
    TR.set_branch("cat_pT", agile::root::integer);
    TR.set_branch("cat_eta", agile::root::integer);
    TR.set_branch("nSingleTracks", agile::root::integer);
    TR.set_branch("nTracks", agile::root::integer);
    TR.set_branch("nTracksAtVtx", agile::root::integer);
    TR.set_branch("nVTX", agile::root::integer);
    TR.set_branch("SV1", agile::root::double_precision);
    TR.set_branch("SV0", agile::root::double_precision);
    TR.set_branch("ip3d_pb", agile::root::double_precision);
    TR.set_branch("ip3d_pu", agile::root::double_precision);
    TR.set_branch("ip3d_pc", agile::root::double_precision);
    TR.set_branch("jfit_efrc", agile::root::double_precision);
    TR.set_branch("jfit_mass", agile::root::double_precision);
    TR.set_branch("jfit_sig3d", agile::root::double_precision);
    TR.set_branch("svp_mass", agile::root::double_precision);
    TR.set_branch("svp_efrc", agile::root::double_precision);
    TR.set_branch("energyFraction", agile::root::double_precision);
    TR.set_branch("mass", agile::root::double_precision);
    TR.set_branch("maxSecondaryVertexRho", agile::root::double_precision);
    TR.set_branch("maxTrackRapidity", agile::root::double_precision);
    TR.set_branch("meanTrackRapidity", agile::root::double_precision);
    TR.set_branch("minTrackRapidity", agile::root::double_precision);
    TR.set_branch("significance3d", agile::root::double_precision);
    TR.set_branch("subMaxSecondaryVertexRho", agile::root::double_precision);
    TR.set_branch("jfit_nvtx", agile::root::integer);
    TR.set_branch("jfit_nvtx1t", agile::root::integer);
    TR.set_branch("jfit_ntrkAtVx", agile::root::integer);

    std::cout << "Pulling dataset from ROOT file...";
    agile::dataframe D = TR.get_dataframe(end - start, start);

    agile::neural_net net;
    net.add_data(D);

    layer_type net_type;
    std::string passed_target = p.get_value<std::string>("type");

    if (passed_target == "regress") net_type = linear;
    else if (passed_target == "multiclass") net_type = softmax;
    else if (passed_target == "binary") net_type = sigmoid;
    else complain("type of target needs to be one of 'regress', 'multiclass', or 'binary'.");

    int i;
    for (i = 0; i < (structure.size() - 2); ++i)
    {
        net.emplace_back(new autoencoder(structure[i], structure[i + 1], sigmoid));
    }
    net.emplace_back(new autoencoder(structure[i], structure[i + 1], sigmoid));
 
    net.model_formula("bottom + charm + light ~ * -pt -eta");

    net.set_learning(learning);
    net.set_regularizer(regularizer);
    net.set_momentum(momentum);
    net.set_batch_size(batch);
    
    net.check(0);

    net.to_yaml(save_file);

    return 0;
}


void config_info()
{
    exit(0);
}

void complain(const std::string &complaint)
{
    std::cerr << "Error: " << complaint << std::endl;
    exit(1);
}

const std::string timestamp(void)  
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}