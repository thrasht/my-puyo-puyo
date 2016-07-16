
class Puyo
{
      private:
              int pos;
              char figure;
              
      public:
             Puyo()
             {
                   pos = 25;      
             }
             int Pos();   
             int Avanza();   
             char Figure();
             void Right();
             void Left();
             void Down();
      
};

int Puyo::Pos()
{
 
       return pos;      
}

int Puyo::Avanza()
{
    pos+=80;
    
    return pos;    
     
}

char Puyo::Figure()
{
    figure = 0x01;
    
    return figure;   
}

void Puyo::Right()
{
     pos++;
     
}

void Puyo::Left()
{
     pos--;
}

void Puyo::Down()
{
     pos+=80;
}





