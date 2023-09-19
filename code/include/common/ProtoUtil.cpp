#include "ProtoUtil.h"
#include <iostream>
#include <ctime>
#include <iostream>
#include <iterator>
#include <locale>
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

  std::string CProtoUtil::Trim(const std::string &s)
  {
    std::string strValue = s;
    if (strValue.empty())
    {
      return strValue;
    }
    std::size_t nCount = strValue.length();
    std::size_t nStartPos = 0;
    for (nStartPos = 0; nStartPos < nCount; nStartPos++)
    {
      if ((s[nStartPos] != ' ')&&
          (s[nStartPos] != '\r') &&
          (s[nStartPos] != '\n'))
      {
        break;
      }
    }

    std::size_t nStopPos = nCount - 1;
    for (nStopPos = nCount - 1; nStopPos > 0; nStopPos--)
    {
      if ((s[nStopPos] != ' ')&&
          (s[nStopPos] != '\r') &&
          (s[nStopPos] != '\n'))
      {
        break;
      }
    }
    return strValue.substr(nStartPos,nStopPos-nStartPos+1);
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
    auto index = strInput.find(":");
    if (index)
    {
      std::string strValue = strInput.substr(index + 1, strInput.length() - index - 1);
      auto addrStartPos = strValue.find("<");
      auto addrEndPos = strValue.find(">");
      if (addrEndPos != std::string::npos && addrStartPos != std::string::npos)
      {
        emailAddr = strValue.substr(addrStartPos + 1, addrEndPos - addrStartPos - 1);
        name = emailAddr;
      }
      else
      {
        emailAddr = strValue;
        name = strValue;
      }
      return true;
    }
    return false;
  }

  std::string CProtoUtil::CreateUserAddrFromNameAndDomain(const std::string strName, const std::string strDomain)
  {
    auto index = strName.find("@");
    if (index != std::string::npos)
    {
      return Trim(strName);
    }
    else
    {
      auto ptIndex = strDomain.find(".");
      if (ptIndex != std::string::npos)
      {
        std::string strTail = strDomain.substr(ptIndex + 1, strDomain.length() - ptIndex - 1);
        return strName + "@" + strTail;
      }
    }
    return "";
  }

  std::time_t CProtoUtil::Now()
  {
    return time(nullptr);
  }

  std::string CProtoUtil::TimeToString(const std::time_t t)
  {
    char timeString[std::size("yyyy-mm-ddThh:mm:ssZ")]={0};
    std::strftime(std::data(timeString), std::size(timeString),
                  "%FT%TZ", std::gmtime(&t));
    return timeString;
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