//#include <iostream>
//#include <string>
//using namespace std;
//
//class File
//{
//public:
//	File(string file_name);
//	virtual ~File() {}
//	string name() const;
//	virtual void open() const = 0;
//	virtual void redisplay() const;
//private:
//	string m_name;
//};
//
//File::File(string file_name):m_name(file_name){}
//
//string File::name() const
//{
//	return m_name;
//}
//
//void File::redisplay() const
//{
//	cout << "refresh the screen";
//}
//
//class TextMsg : public File
//{
//public:
//	TextMsg(string text_name);
//	~TextMsg();
//	void open() const;
//};
//
//TextMsg::TextMsg(string text_name) :File(text_name) {}
//
//TextMsg::~TextMsg()
//{
//	cout << "Destroying " << name() << ", a text message" << endl;
//}
//
//void TextMsg::open() const
//{
//	cout << "open text message";
//}
//
//class Video :public File
//{
//public:
//	Video(string video_name, int video_time);
//	~Video();
//	void open() const;
//	void redisplay() const;
//private:
//	int m_time;
//};
//
//Video::Video(string video_name, int video_time) :File(video_name), m_time(video_time){}
//
//Video::~Video()
//{
//	cout << "Destroying " << name() << ", a video" << endl;
//}
//
//void Video::open() const
//{
//	cout << "play " << m_time << " second video";
//}
//
//void Video::redisplay() const
//{
//	cout << "replay video";
//}
//
//class Picture :public File
//{
//public:
//	Picture(string picture_name);
//	~Picture();
//	void open() const;
//};
//
//Picture::Picture(string picture_name) :File(picture_name) {}
//
//Picture::~Picture()
//{
//	cout << "Destroying the picture " << name() << endl;
//}
//
//void Picture::open() const
//{
//	cout << "show picture";
//}
//
//void openAndRedisplay(const File* f)
//{
//	cout << f->name() << ": ";
//	f->open();
//	cout << endl << "Redisplay: ";
//	f->redisplay();
//	cout << endl;
//}
//
//int main()
//{
//	File* files[4];
//	files[0] = new TextMsg("fromFred.txt");
//	// Videos have a name and running time
//	files[1] = new Video("goblin.mpg", 3780);
//	files[2] = new Picture("kitten.jpg");
//	files[3] = new Picture("baby.jpg");
//
//	for (int k = 0; k < 4; k++)
//		openAndRedisplay(files[k]);
//
//	// Clean up the files before exiting
//	cout << "Cleaning up." << endl;
//	for (int k = 0; k < 4; k++)
//		delete files[k];
//}