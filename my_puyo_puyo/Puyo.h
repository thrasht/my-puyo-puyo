#pragma once

class Puyo
{
	private:
		bool active;
		int pos;
		char figure;
		int color;
		int partner;

	public:
		Puyo();
		Puyo(int);
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
		void EditFigure(char);
		void EditColor(int);
		void EditPos(int);
		int Partner();
		void EditPartner(int);

};


