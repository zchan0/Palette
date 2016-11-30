class Utils
{
	public:
		static Utils& getInstance();
		void RGBtoHSV(double r, double g, double b, double &h, double &s, double &v);
		void HSVtoRGB(double h, double s, double v, double &r, double &g, double &b);
	
	private:
		Utils() {}
		Utils(const Utils&);
		Utils& operator=(const Utils&);
};
