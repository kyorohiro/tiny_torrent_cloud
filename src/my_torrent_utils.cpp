// #include "my_torrent.hpp"
#include <iostream>
#include <unistd.h>
#include <vector>

#include <libtorrent/bencode.hpp>
#include <libtorrent/create_torrent.hpp>
#include <libtorrent/torrent_info.hpp>

namespace my_torrent {

std::string to_absolute_path(std::string args);
std::string extract_parent_path(std::string filepath);
void print_usage(std::string execute_file_name);

std::string tracker_address = "";
//     std::vector<char> torrentfile_source;
//    lt::bencode(std::back_inserter(torrentfile_source), entry_info); //
//    metainfo -> binary
lt::create_torrent gen_torrentfile(std::back_insert_iterator<std::vector<char, std::allocator<char>>> out, std::string target_file_path) {
  std::string target_tracker_address = "";

  //
  // setup file storage
  std::string source_absolute_path = to_absolute_path(target_file_path);
  lt::file_storage fs;
  lt::create_flags_t flags = {};
  lt::add_files(fs, source_absolute_path, flags);

  //
  // create torrent creator
  lt::create_torrent torrent(fs, 16 * 1024);
  if (target_tracker_address.length() > 0) {
    torrent.add_tracker(target_tracker_address);
  }
  //
  // add hash info
  lt::set_piece_hashes(torrent, extract_parent_path(source_absolute_path));

  //
  // generate metadata
  lt::entry entry_info = torrent.generate();

  //
  // convert to bencoding
  //std::vector<char> torrentfile_source;
  lt::bencode(out ,//std::back_inserter(torrentfile_source),
              entry_info); // metainfo -> binary
  //std::ofstream o;         // ofstream is the class for fstream package
  //try {
  //  o.open(output_file_path);
  //  o.write(torrentfile_source.data(),
  //          torrentfile_source.size()); // binary -> stdout
  //} catch (std::exception &exc) {
  //  ;
  //}
  //o.close();
  return torrent;
}

void print_usage(std::string args0) {
  std::cerr << "" << args0 << " <filename> [ -a <tracker address> ]"
            << std::endl;
}

std::string to_absolute_path(std::string path) {
  if (path.length() == 0) {
    throw std::runtime_error("failed to current_dir");
  }
  if (path[0] == '/') {
    return path;
  }
  char current_dir_path[2056];
  auto ret = getcwd(current_dir_path, sizeof(current_dir_path));
  if (ret == NULL) {
    std::cerr << "failed to current_dir ";
    throw std::runtime_error("failed to current_dir");
  }
  if (0 == path.compare(0, 2, "./")) {
    path = path.substr(2);
  }
  return std::string(current_dir_path) + "/" + path;
}

std::string extract_parent_path(std::string filepath) {
  if (filepath.empty()) {
    return filepath;
  }
  int pos = filepath.find_last_of("/");

  return filepath.substr(0, pos);
}

} // namespace my_torrent
