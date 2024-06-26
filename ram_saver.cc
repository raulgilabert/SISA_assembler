#include <cstdio>
#include <iostream>
#include <fstream>
#include "constants.h"
#include "args_getter.h"


void saveRam(uint8_t* ram, size_t len, char* path, const uint8_t &flags)
{
	bool div = flags & MEM_DIV; 
  bool raw_hex = flags & RAW_HEX;

	if(!div)
	{
		std::ofstream outputFile(path);
		if(!outputFile)
		{
			printf("%s: Couldn't write to output file\n", prog_name);
			exit(1);
		}

    if(raw_hex)
    {
      for(size_t i = 0; i<len; i+=2)
	    {
		    uint8_t b0 = ram[i];
		    uint8_t b1 = ram[i+1];
        uint16_t v = b0 | (b1 << 8);

        char line_buff[8];

        sprintf(line_buff, "0x%04X\n", v);
        outputFile.write(line_buff,7); 
	    }   
    }
    else
		  outputFile.write((char*)ram,len);

		outputFile.flush();
		outputFile.close();
		return;
	}
	
	std::string path_str = std::string(path);
	std::string ext;

	std::string::size_type last_dot_pos = path_str.find_last_of('.');

	if(last_dot_pos == std::string::npos) ext = "";
	else{
		ext = path_str.substr(last_dot_pos);
		path_str = path_str.substr(0,last_dot_pos);
	}

	std::ofstream of0(path_str + '0' + ext);
	std::ofstream of1(path_str + '1' + ext);
	
	if(!of0 || !of1)
	{
		printf("%s: Couldn't write to output files\n", prog_name);
		exit(1);
	}

  if(raw_hex)
  {
    for(size_t i = 0; i<len; i+=2)
	  {
		  uint8_t b0 = ram[i];
		  uint8_t b1 = ram[i+1];
      char line_buff[6];
      sprintf(line_buff, "0x%02X\n", b0);
      of0.write(line_buff,5); 
      
      sprintf(line_buff, "0x%02X\n", b1);
      of1.write(line_buff,5); 
	  }
  }
  else
  {
    for(size_t i = 0; i<len; i+=2)
    {
      uint8_t b0 = ram[i];
      uint8_t b1 = ram[i+1];	

      of0.write((char*)&b0,1);
      of1.write((char*)&b1,1);
    }
  }

	of0.close();
	of1.close();	
}
