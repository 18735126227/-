#pragma once
#ifndef __MY_PACK_AND_ENCODE__
#define __MY_PACK_AND_ENCODE__
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <WINDOWS.H>
#include <tchar.h>
#include <stdio.h>
#include <io.h>
#include <sys/stat.h>
#include <wchar.h>
#include "aesfile.h"
#include "my_tar.h"
#include "bundle.h"



void file_to_folder(std::string src_path, std::string dst_folder);
LPCWSTR stringToLPCWSTR(std::string orig);
void my_tar(std::string source, std::string target);
void my_untar(std::string source, std::string target);
void my_pack(std::string source, std::string target);
void my_unpack(std::string source, std::string target);
void my_encode(std::string source, std::string target, const char* key, aes::AESBIT Aes_bit);
void my_decode(std::string source, std::string target, const char* key, aes::AESBIT Aes_bit);
void removeDir(std::string dirPath);
void my_packup(std::string target_folder,std::vector <std::string> cho, const char* key,int aes_flag,int mode_flag);
void my_restore(std::string target_folder, std::vector <std::string> cho, const char* key, int aes_flag);

class CRC32 {
public:
    CRC32();
    uint32_t compute(const std::string filename);
private:
    uint32_t crc_table[256];
    void generate_crc_table();
};

void save_crc_to_txt(const std::string filename, const std::string targetFolder);

std::string get_file_name(const std::string& filepath);
std::string exclude_file_type(const std::string& filepath);
uint32_t read_from_txt(const std::string& filename);
#endif