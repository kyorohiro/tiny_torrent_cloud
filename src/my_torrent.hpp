
#ifndef _MY_TORRENT
#define _MY_TORRENT

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <libtorrent/create_torrent.hpp>
namespace my_torrent {

struct IpInfo {
  std::string ip_address;
  std::string country;
  std::string domain;
  std::string unique_id;
  std::string type;
};

extern std::string _downloaded_file_path;
std::string make_magnet_link(std::vector<char> binary);
void setup(std::string bind_address, int upload_max, int download_max,
           std::string downloaded_file_path);
void listen();
void add_magnetlink(std::string key, std::string magnetlink);
void add_torrentfile(std::string key, std::string path);

lt::create_torrent gen_torrentfile(
    std::back_insert_iterator<std::vector<char, std::allocator<char>>> out,
    std::string target_file_path);
// void gen_torrentfile(std::string torrent_file_path, std::string
// data_file_path);
void extract_ip_list_from_log(std::string log);
void remove_magnetlink(std::string key);
void terminate();
} // namespace my_torrent

#endif
