class Utils
{
	public:
		static Utils& getInstance();
		double round(double number, unsigned int bits);
		void insert(std::vector<double> &v, double val);
		void RGBtoHSV(double r, double g, double b, double &h, double &s, double &v);
		void HSVtoRGB(double h, double s, double v, double &r, double &g, double &b);
	
	private:
		Utils() {}
		Utils(const Utils&);
		Utils& operator=(const Utils&);
};
