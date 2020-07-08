#pragma once
#include "Point.h"

namespace MachanismAnalysis {
	namespace Core {
		public ref class Caculator {
		public:
			int pointsNum;
			int rodsNum;

			void BarKinematic(int n1, int n2, int k, double r1, double gam);
			void RRRKinematic(int m, int n1, int n2, int n3, int k1, int k2, double r1, double r2);
			void RPRKinematic(int m, int n1, int n2, int k1, int k2, double r1, double% r2, double% vr2, double% ar2);
			void RRPKinematic(int m, int n1, int n2, int n3, int k1, int k2, int k3, double r1, double% r2, double% vr2, double% ar2);
			
			/// <summary>
			/// ���㷨���λ�á��ٶȡ����ٶ�   [��Bark��ͬ]
			/// </summary>
			/// <param name="nBasic">����ID</param>
			/// <param name="nToCompute">�����ID</param>
			/// <param name="p">��ID</param>
			/// <param name="len">����</param>
			void BasicPointKinematic(int nBasic, int nToCompute, int p, double len);


			// void Caculate();

			/* Getters And Setters */
			void SetPointsPosition(int n, double x, double y);
			void SetPointsVelocity(int n, double x, double y);
			void SetPointsAcceleration(int n, double x, double y);
			Point^ GetPointsPosition(int n);
			Point^ GetPointsVelocity(int n);
			Point^ GetPointsAcceleration(int n);
			void SetRodsAngularDisplacement(int k, double x);
			void SetRodsAngularVelocity(int k, double x);
			void SetRodsAngularAcceleration(int k, double x);
			double GetRodsAngularDisplacement(int k);
			double GetRodsAngularVelocity(int k);
			double GetRodsAngularAcceleration(int k);
			
			void ConfigurePoint(int n, Point^ pos, Point^ vel, Point^ acc);
			void ConfigurePoint(int n, double posX, double posY, double velX, double velY, double accX, double accY);
			void ConfigureRod(int k, double pos, double vel, double acc);

			void PrintPointInfo(int n);
			void PrintRodInfo(int k);



			Caculator();
			Caculator(int pNum, int rNum);
			~Caculator();


		private:
			double(*pointsPosition)[3];
			double(*pointsVelocity)[3];
			double(*pointsAcceleration)[3];

			double* rodsAngularDisplacement;
			double* rodsAngularVelocity;
			double* rodsAngularAcceleration;

		};


	}



}


