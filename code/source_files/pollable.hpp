class Pollable {
	virtual int readData() = 0;
	virtual int writeData() = 0;
	virtual bool isListener() const = 0;
};

class ListenPoll : public Pollable {
	struct pollfd _pfd;

	bool isListener() const;
};

class SocketPoll : public Pollable {
	std::vector<char> _buffer;
	std::string _data;
	int _bytes;
	int _left;
	int _total;
	int _fd;
	
	int readData();
	int writeData();
	bool isListener() const;
	bool isSocket() const;
	bool isFile() const;
};

class FilePoll : public Pollable {
	std::vector<char> _buffer;
	std::string _data;
	int _bytes;
	int _left;
	int _total;
	int _fd;
	
	int readData();
	int writeData();
	bool isListener() const;
};
