#pragma once

class Puyo
{
	private:
		bool active;
		int pos;
		char figure;
		int color;

	public:
		Puyo(void);
		Puyo(bool);
		~Puyo(void);
		int Pos();   
		int Avanza();   
		char Figure();
		void Right();
		void Left();
		void Down();
		bool Check();
		bool ChangeState(bool);
		int Color();

};


