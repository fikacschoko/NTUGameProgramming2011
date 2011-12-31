#include <TheFlyWin32.h>

class FPSview
{
	private:
		float pos_begin[3];
		int oldX, oldY;
		static FPSview *class_now;
	public:
		VIEWPORTid vID;
		OBJECTid cFPS;
		OBJECTid FPSstandID;
		FPSview( WORLDid , SCENEid ,OBJECTid , int , int , int , int );
		static void turn( WORLDid , int , int  );
		void move();
		void render();
		//must set before bind "turn()" with mouse
		static void setClsNow( FPSview* cls ){
			FPSview::class_now = cls;
		}
		static FPSview* getClsNow(){
			//return NULL;
			return FPSview::class_now;
		}
};