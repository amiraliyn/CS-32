class File
{
public:
	File(string file_name);
	virtual ~File() {}
	string name() const;
	virtual void open() const = 0;
	virtual void redisplay() const;
private:
	string m_name;
};

File::File(string file_name) :m_name(file_name) {}

string File::name() const
{
	return m_name;
}

void File::redisplay() const
{
	cout << "refresh the screen";
}

class TextMsg : public File
{
public:
	TextMsg(string text_name);
	~TextMsg();
	void open() const;
};

TextMsg::TextMsg(string text_name) :File(text_name) {}

TextMsg::~TextMsg()
{
	cout << "Destroying " << name() << ", a text message" << endl;
}

void TextMsg::open() const
{
	cout << "open text message";
}

class Video :public File
{
public:
	Video(string video_name, int video_time);
	~Video();
	void open() const;
	void redisplay() const;
private:
	int m_time;
};

Video::Video(string video_name, int video_time) :File(video_name), m_time(video_time) {}

Video::~Video()
{
	cout << "Destroying " << name() << ", a video" << endl;
}

void Video::open() const
{
	cout << "play " << m_time << " second video";
}

void Video::redisplay() const
{
	cout << "replay video";
}

class Picture :public File
{
public:
	Picture(string picture_name);
	~Picture();
	void open() const;
};

Picture::Picture(string picture_name) :File(picture_name) {}

Picture::~Picture()
{
	cout << "Destroying the picture " << name() << endl;
}

void Picture::open() const
{
	cout << "show picture";
}