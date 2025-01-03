#include "my_pack_and_encode.h"

namespace fs = std::filesystem;

void my_packup(std::string target_folder, std::vector <std::string> cho, const char* key, int aes_flag, int mode_flag)
{
    aes::AESBIT key_mode = (aes_flag == 1 ? aes::AES_128 : (aes_flag == 2 ? aes::AES_192 : aes::AES_256));
    time_t timep;
    time(&timep);
    std::string filename = (std::string)ctime(&timep);
    replace(filename.begin(), filename.end(), ':', '_');
    filename.erase(filename.end() - 1);
    
    std::string temp_folder = target_folder + "\\" + filename;

    switch (mode_flag)
    {
    case 1:
        for (auto path : cho)
        {
            file_to_folder(path, target_folder);
            save_crc_to_txt(path, target_folder);
        }
        break;

    case 2:   //tar
        CreateDirectory(stringToLPCWSTR(temp_folder), NULL);
        for (auto path : cho)
            file_to_folder(path, temp_folder);

        my_tar(temp_folder, target_folder + "\\" + filename + ".tar");
        save_crc_to_txt(target_folder + "\\" + filename + ".tar", target_folder);

        removeDir(temp_folder);
        RemoveDirectory(stringToLPCWSTR(temp_folder));
        break;

    case 3:  //tar.lz
        CreateDirectory(stringToLPCWSTR(temp_folder), NULL);
        for (auto path : cho)
            file_to_folder(path, temp_folder);
        my_tar(temp_folder, (std::string)target_folder + "\\" + filename + ".tar");
        removeDir(temp_folder);
        RemoveDirectory(stringToLPCWSTR(temp_folder));
        my_pack((std::string)target_folder + "\\" + filename + ".tar", (std::string)target_folder + "\\" + filename + ".tar.lz");
        save_crc_to_txt((std::string)target_folder + "\\" + filename + ".tar.lz", target_folder);
        remove(((std::string)target_folder + "\\" + filename + ".tar").c_str());
        break;

    case 4:  //tar.aes
        CreateDirectory(stringToLPCWSTR(temp_folder), NULL);
        for (auto path : cho)
            file_to_folder(path, temp_folder);
        my_tar(temp_folder, (std::string)target_folder + "\\" + filename + ".tar");
        removeDir(temp_folder);
        RemoveDirectory(stringToLPCWSTR(temp_folder));
        my_encode((std::string)target_folder + "\\" + filename + ".tar", (std::string)target_folder + "\\" + filename + ".tar.aes", key, key_mode);
        save_crc_to_txt((std::string)target_folder + "\\" + filename + ".tar.aes", target_folder);
        remove(((std::string)target_folder + "\\" + filename + ".tar").c_str());
        break;

    case 5:  //tar.aes.lz
        CreateDirectory(stringToLPCWSTR(temp_folder), NULL);
        for (auto path : cho)
            file_to_folder(path, temp_folder);
        my_tar(temp_folder, (std::string)target_folder + "\\" + filename + ".tar");
        removeDir(temp_folder);
        RemoveDirectory(stringToLPCWSTR(temp_folder));
        my_pack((std::string)target_folder + "\\" + filename + ".tar", target_folder + "\\" + filename + ".tar.lz");
        remove(((std::string)target_folder + "\\" + filename + ".tar").c_str());
        my_encode((std::string)target_folder + "\\" + filename + ".tar.lz", (std::string)target_folder + "\\" + filename + ".tar.lz.aes", key, key_mode);
        save_crc_to_txt((std::string)target_folder + "\\" + filename + ".tar.lz.aes", target_folder);
        remove(((std::string)target_folder + "\\" + filename + ".tar.lz").c_str());
        break;

    default:
        break;
    }
}

void my_restore(std::string target_folder, std::vector <std::string> cho, const char* key, int aes_flag)
{
    aes::AESBIT key_mode = (aes_flag == 1 ? aes::AES_128 : (aes_flag == 2 ? aes::AES_192 : aes::AES_256));
    for (auto path : cho)
    {
        file_to_folder(path, target_folder);
        //1.获取不带路径的文件名
        std::string::size_type iPos = path.find_last_of('\\') + 1;
        std::string filename = path.substr(iPos, path.length() - iPos);
        //2.获取不带后缀的文件名
        std::string name = filename.substr(0, filename.rfind("."));
        //3.获取后缀名
        std::string suffix_str = filename.substr(filename.find_last_of('.') + 1);
        std::string new_file = (std::string)(target_folder + "\\" + filename);

        if (suffix_str != "aes" && suffix_str != "lz" && suffix_str != "tar") continue;

        while (suffix_str == "aes" || suffix_str == "lz")
        {
            if (suffix_str == "aes") {
                my_decode(new_file, std::string(target_folder + "\\" + name), key, key_mode);
                remove(new_file.c_str());
            }
            else if (suffix_str == "lz") {
                my_unpack(new_file, std::string(target_folder + "\\" + name));
                remove(new_file.c_str());
            }
            path = std::string(target_folder + "\\" + name);
            //1.获取不带路径的文件名
            iPos = path.find_last_of('\\') + 1;
            filename = path.substr(iPos, path.length() - iPos);
            //2.获取不带后缀的文件名
            name = filename.substr(0, filename.rfind("."));
            //3.获取后缀名
            suffix_str = filename.substr(filename.find_last_of('.') + 1);
            new_file = (std::string)(target_folder + "\\" + filename);
        }

        my_untar(new_file, target_folder);
        remove(new_file.c_str());

    }
}


void file_to_folder(std::string src_path, std::string dst_folder)
{
    std::string dst_path = dst_folder + '\\' + fs::path(src_path).filename().string();
    std::ifstream src_file(src_path, std::ios::binary);
    std::ofstream dst_file(dst_path, std::ios::binary);
    dst_file << src_file.rdbuf();
    src_file.close();
    dst_file.close();
    std::cout << src_path << "File copied to " << dst_path << std::endl;
}

LPCWSTR stringToLPCWSTR(std::string orig)//将一个 std::string 类型的字符串转换为 Windows API 中使用的宽字符字符串类型 LPCWSTR
{
    /*
    size_t origsize = orig.length() + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() - 1));
    mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

    return wcstring;
    */
    size_t origsize = orig.length();
    int wLen = ::MultiByteToWideChar(CP_UTF8,
        0,
        orig.c_str(),
        -1,
        NULL,
        0);
    wchar_t* buffer = new wchar_t[wLen + 1];
    memset(buffer, 0, (wLen + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, orig.c_str(), origsize, (LPWSTR)buffer, wLen);
    return buffer;
}

void my_tar(std::string source, std::string target)
{
    if (source[0] == '?') source.erase(source.begin());
    if (target[0] == '?') target.erase(target.begin());
    const char* p = source.data();
    const char* q = target.data();
    my_Tar::getInstance()->tar(p, q);
}

void my_untar(std::string source, std::string target)
{
    if (source[0] == '?') source.erase(source.begin());
    if (target[0] == '?') target.erase(target.begin());
    const char* p = source.data();
    const char* q = target.data();
    my_Tar::getInstance()->detar(p, q);
}

void my_pack(std::string source, std::string target)
{
    std::ifstream ifs;
    ifs.open(source, std::ios::binary);//打开原始文件

    ifs.seekg(0, std::ios::end);//跳转读写位置到文件末尾
    size_t fsize = ifs.tellg();//获取末尾偏移量（也就是获取文件大小）
    ifs.seekg(0, std::ios::beg);//跳转会起始

    std::string body;
    body.resize(fsize);
    ifs.read(&body[0], fsize);//读取文件数据到body中

    //以LZIP的方式压缩文件数据
    std::string packed = bundle::pack(bundle::LZIP, body);

    std::ofstream ofs;
    ofs.open(target, std::ios::binary);//打开压缩包文件
    ofs.write(&packed[0], packed.size());//将压缩后的数据写入压缩包文件

    ifs.close();
    ofs.clear();
    SetFileAttributes(stringToLPCWSTR(target), FILE_ATTRIBUTE_NORMAL);
}

void my_unpack(std::string source, std::string target)
{
    std::ifstream ifs;
    ifs.open(source, std::ios::binary);
    ifs.seekg(0, std::ios::end);
    size_t fsize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::string body;
    body.resize(fsize);
    ifs.read(&body[0], fsize);
    ifs.close();

    std::string unpacked = bundle::unpack(body);
    std::ofstream ofs;
    ofs.open(target, std::ios::binary);
    ofs.write(&unpacked[0], unpacked.size());
    ofs.close();
    SetFileAttributes(stringToLPCWSTR(target), FILE_ATTRIBUTE_NORMAL);
}


void my_encode(std::string source, std::string target, const char* key, aes::AESBIT Aes_bit)
{

    aes Aes;
    Aes.setKey((unsigned char*)key, Aes_bit);
    Aes.encryptFile(source.c_str(), target.c_str());
    SetFileAttributes(stringToLPCWSTR(target), FILE_ATTRIBUTE_NORMAL);
}

void my_decode(std::string source, std::string target, const char* key, aes::AESBIT Aes_bit)
{
    aes Aes;
    Aes.setKey((unsigned char*)key, Aes_bit);
    Aes.decryptFile(source.c_str(), target.c_str());

    SetFileAttributes(stringToLPCWSTR(target), FILE_ATTRIBUTE_NORMAL);
}

void  removeDir(std::string dirPath)
{
    struct _finddata_t fb;   // 查找相同属性文件的存储结构体
    std::string path;
    long long  handle; // 注意此处需要long*2
    int   noFile;            // 对系统隐藏文件的处理标记
    noFile = 0;
    handle = 0;
    struct stat s;

    path = dirPath + "/*";

    handle = _findfirst(path.c_str(), &fb);
    // 找到第一个匹配的文件
    if (handle != 0)
    {
        // 当可以继续找到匹配的文件，继续执行
        while (0 == _findnext(handle, &fb))
        {
            // windows下，常有个系统文件，名为“..”,对它不做处理
            noFile = strcmp(fb.name, "..");

            if (0 != noFile)
            {
                path = dirPath + "/" + fb.name;

                // 属性值为16，则说明是文件夹，迭代
                if (fb.attrib == 16)
                {
                    removeDir(path);
                }
                // 非文件夹的文件，直接删除。对文件属性值的情况没做详细调查，可能还有其他情况。
                else
                {
                    remove(path.c_str());
                }
            }
        }
        //删除空目录
        _wrmdir((wchar_t*)dirPath.c_str());
        // 关闭文件夹，只有关闭了才能删除。找这个函数找了很久，标准c中用的是closedir
        // 经验介绍：一般产生Handle的函数执行后，都要进行关闭的动作。
        _findclose(handle);
    }
}










CRC32::CRC32()
{
    generate_crc_table();
}

uint32_t CRC32::compute(const std::string filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    uint32_t crc = 0xFFFFFFFF; // 初始化 CRC 值
    char buffer[1024];

    while (file.read(buffer, sizeof(buffer))) {
        for (std::streamsize i = 0; i < file.gcount(); ++i) {
            crc = (crc >> 8) ^ crc_table[(crc ^ static_cast<unsigned char>(buffer[i])) & 0xFF];
        }
    }

    // 处理余下的字节
    for (std::streamsize i = 0; i < file.gcount(); ++i) {
        crc = (crc >> 8) ^ crc_table[(crc ^ static_cast<unsigned char>(buffer[i])) & 0xFF];
    }

    return ~crc; // 取反得到最终的 CRC 值
}

void CRC32::generate_crc_table() {
    for (uint32_t i = 0; i < 256; ++i) {
        uint32_t crc = i;
        for (uint32_t j = 0; j < 8; ++j) {
            crc = (crc >> 1) ^ ((crc & 1) ? 0xEDB88320 : 0); // 多项式
        }
        crc_table[i] = crc;
    }

}

void save_crc_to_txt(const std::string filename, const std::string targetFolder) //第一个参数时待校验文件，第二个参数时保存结果的文件
{

    std::string output_file = targetFolder + "\\" + get_file_name(filename) + ".txt";
    std::ofstream txt_file(output_file); // 默认即为文本模式;

    if (!txt_file.is_open()) {
        throw std::runtime_error("Unable to open output file: " + output_file);
    }
    CRC32 crc_calculator;

    uint32_t crc_value = crc_calculator.compute(filename);
    txt_file << std::hex << crc_value; // 保存为十六进制
    txt_file.close();
    SetFileAttributes(stringToLPCWSTR(output_file), FILE_ATTRIBUTE_READONLY);//设置只读
}

std::string get_file_name(const std::string& filepath) {
    size_t lastSlashPos = filepath.find_last_of("/\\"); // 查找最后一个 '/' 或 '\'

    std::string filename = (lastSlashPos == std::string::npos) ? filepath : filepath.substr(lastSlashPos + 1);

    return filename;
}

std::string exclude_file_type(const std::string& filename) {

    // 查找最后一个 '.'，以便去除扩展名
    size_t lastDotPos = filename.find_last_of(".");
    // 去除扩展名
    return (lastDotPos == std::string::npos) ? filename : filename.substr(0, lastDotPos);
}

uint32_t read_from_txt(const std::string& filename) {
    std::ifstream file(filename);
    uint32_t crc_results;

    // 检查文件是否成功打开
    if (!file.is_open()) {
        return 941871365;
    }
    std::string line;
    std::getline(file, line);
    std::stringstream ss(line);
    ss >> std::hex >> crc_results;
    file.close(); // 关闭文件
    return crc_results;
}



