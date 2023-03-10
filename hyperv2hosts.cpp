#include <cstdio>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <CLI/CLI.hpp>

using namespace std;

struct Entry {
    string name;
    string ip;
};

void query_hyperv (string const &name, vector<Entry> *entries, string const &temp_path) {
    // ideally we want to get rid of the temp file
    // by using something similar to the posix popen
    string cmd = fmt::format("powershell.exe  -command \"Get-VMNetworkAdapter -VMName {} | Select VMName, IPAddresses\" > {}", name, temp_path);
    system(cmd.c_str());
    entries->clear();
    ifstream is(temp_path);
    string line;
    while (is) {
        getline(is, line);
        auto end_host = line.find(' ');
        auto left_brace = line.find('{');
        auto end_ip = line.find(',', left_brace);

        if (end_host == line.npos) continue;
        if (left_brace == line.npos) continue;
        if (end_ip == line.npos) continue;

        Entry e;
        e.name = line.substr(0, end_host);
        e.ip = line.substr(left_brace + 1, end_ip - left_brace - 1);
        entries->push_back(e);
    }
}

void load_hosts_lines (vector<string> *lines, string const &path, string const &magic) {
    ifstream is(path);
    string line;
    lines->clear();
    for (;;) {
        getline(is, line, '\n');
	if (!is) break;
        if (line.find(magic) != line.npos) continue;
        lines->push_back(line);
    }
}

string get_time_string () {
    time_t timetoday;
    char timebuf[BUFSIZ] = {0};
    time (&timetoday);    
    asctime_s(timebuf, BUFSIZ, localtime(&timetoday));
    return string(timebuf);
}

int main (int argc, char *argv[]) {
    string output_path = "c:\\windows\\system32\\drivers\\etc\\hosts";
    string temp_path = "hyperv2hosts.tmp";
    string magic = "[HYPERV2HOSTS]";
    string host = "linux";
    string host_fmt = "{}";
    bool dry = false;
    {
        CLI::App cli{"hyperv2hosts"};
        cli.add_option("-n,--name", host, "host name");
        cli.add_option("-o,--output", output_path, "path to the host file");
        cli.add_option("--temp", temp_path, "temporary file containing hyperv output");
        cli.add_option("--magic", magic, "pattern used to identify added lines by this program in host file.");
        cli.add_option("--host-fmt", host_fmt, "how to format name, {} is replaced with guest name.");
        cli.add_flag("--dry", dry, "dry run, print instead of updating host file.");
        CLI11_PARSE(cli, argc, argv);
    }

    vector<Entry> entries;
    query_hyperv(host, &entries, temp_path);

    vector<string> lines;
    load_hosts_lines(&lines, output_path, magic);

    ostream *os = &cout;
    ofstream output;
    if (!dry) {
        output.open(output_path);
        os = &output;
    }


    for (auto const &line: lines) {
        (*os) << line << endl;
    }

    (*os) << "# " << magic << " Updated at " << get_time_string();
    for (auto const &e: entries) {
        (*os) << e.ip << "\t" << fmt::format(host_fmt, e.name) << " # " << magic << endl;
    }

    if (!dry) {
        output.close();
    }

    return 0;
}
