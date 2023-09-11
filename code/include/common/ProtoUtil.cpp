#include "ProtoUtil.h"
#include <iostream>
namespace tiny_email
{
  // Copy from QT
  std::vector<std::string> CProtoUtil::SplitStringByLine(const std::string &strValue)
  {
    std::vector<std::string> strvec;

    std::string::size_type pos1, pos2;
    std::string strSplit = "\r\n";
    pos2 = strValue.find(strSplit);
    pos1 = 0;
    while (std::string::npos != pos2)
    {
      strvec.push_back(strValue.substr(pos1, pos2 - pos1 + 2));
      pos1 = pos2 + 2;
      pos2 = strValue.find(strSplit, pos1);
    }
    // strvec.push_back(strValue.substr(pos1));
    return strvec;
  }

  std::string &CProtoUtil::Trim(std::string &s)
  {
    if (s.empty())
    {
      return s;
    }
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
  }

  bool CProtoUtil::SplitLine(const std::string &strInput, int &code, std::string &value, bool &bFinish)
  {
    {
      std::string::size_type pos2;
      std::string strSplit = "-";
      pos2 = strInput.find(strSplit);
      if (std::string::npos != pos2 && pos2 == 3)
      {
        std::string strCode = strInput.substr(0, pos2);
        code = std::stoi(strCode);
        value = strInput.substr(pos2, strInput.length() - pos2);
        bFinish = false;
        return true;
      }
    }
    {
      std::string::size_type pos2;
      std::string strSplit = " ";
      pos2 = strInput.find(strSplit);
      if (std::string::npos != pos2 && pos2 == 3)
      {
        std::string strCode = strInput.substr(0, pos2);
        code = std::stoi(strCode);
        value = strInput.substr(pos2, strInput.length() - pos2);
        std::cout << "Finish\t" << code << "\t" << value << std::endl;
        bFinish = true;
        return true;
      }
    }
    return false;
  }

  // Base 64 Begin
  static const std::string base64_chars =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz"
      "0123456789+/";

  static inline bool is_base64(unsigned char c)
  {
    return (isalnum(c) || (c == '+') || (c == '/'));
  }

  static std::string base64_encode(unsigned char const *bytes_to_encode, std::size_t in_len)
  {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--)
    {
      char_array_3[i++] = *(bytes_to_encode++);
      if (i == 3)
      {
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (i = 0; (i < 4); i++)
          ret += base64_chars[char_array_4[i]];
        i = 0;
      }
    }

    if (i)
    {
      for (j = i; j < 3; j++)
        char_array_3[j] = '\0';

      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

      for (j = 0; (j < i + 1); j++)
        ret += base64_chars[char_array_4[j]];

      while ((i++ < 3))
        ret += '=';
    }

    return ret;
  }

  std::string CProtoUtil::Base64Encode(const std::string strInput)
  {
    return base64_encode((unsigned char const *)(strInput.c_str()), strInput.length());
  }

  bool CProtoUtil::ParseFromToString(const std::string strInput, std::string &name, std::string &emailAddr)
  {
    if (strInput.find("From: ") != std::string::npos ||
        strInput.find("To: ") != std::string::npos)
    {
      auto addrStartPos = strInput.find("<");
      auto addrEndPos = strInput.find(">");
      emailAddr = strInput.substr(addrStartPos + 1, addrEndPos - addrStartPos - 1);
      name = emailAddr;
      return true;
    }
    return false;
  }
  std::string CProtoUtil::Base64Decode(const std::string encoded_string)
  {
    std::size_t in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
    {
      char_array_4[i++] = encoded_string[in_];
      in_++;
      if (i == 4)
      {
        for (i = 0; i < 4; i++)
          char_array_4[i] = static_cast<unsigned char>(base64_chars.find(char_array_4[i]));

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (i = 0; (i < 3); i++)
          ret += char_array_3[i];
        i = 0;
      }
    }

    if (i)
    {
      for (j = 0; j < i; j++)
        char_array_4[j] = static_cast<unsigned char>(base64_chars.find(char_array_4[j]));

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

      for (j = 0; (j < i - 1); j++)
        ret += char_array_3[j];
    }

    return ret;
  }
} // namespace cpp_email