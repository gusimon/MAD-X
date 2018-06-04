// mad_synrad.cpp  SR photon spectrum generator H.Burkhardt CERN-OPEN-2007-018
// also used in Geant4  G4SynchrotronRadiation
// modified here to make routine name and argument FORTRAN call compatible

#include <cmath>

inline double Chebyshev(double a,double b,const double c[],int m,double x)
{
  double y;
  double y2=2.0*(y=(2.0*x-a-b)/(b-a)); // Change of variable.
  double d=0,dd=0,sv;
  for (int j=m-1;j>=1;j--) // Clenshaw's recurrence.
  {
    sv=d;
    d=y2*d-dd+c[j];
    dd=sv;
  }
  return y*d-dd+0.5*c[0];
}

extern "C" double invsynfracint_(double *fortx)
{
  double x=fortx[0];
  // from 0 to 0.7
  static const double aa1=0  ,aa2=0.7;
  static const int ncheb1=27;
  static const double cheb1[] =
  {
    1.22371665676046468821,0.108956475422163837267,0.0383328524358594396134,0.00759138369340257753721,
    0.00205712048644963340914,0.000497810783280019308661,0.000130743691810302187818,0.0000338168760220395409734,
    8.97049680900520817728e-6,2.38685472794452241466e-6,6.41923109149104165049e-7,1.73549898982749277843e-7,
    4.72145949240790029153e-8,1.29039866111999149636e-8,3.5422080787089834182e-9,9.7594757336403784905e-10,
    2.6979510184976065731e-10,7.480422622550977077e-11,2.079598176402699913e-11,5.79533622220841193e-12,
    1.61856011449276096e-12,4.529450993473807e-13,1.2698603951096606e-13,3.566117394511206e-14,1.00301587494091e-14,
    2.82515346447219e-15,7.9680747949792e-16};
  //   from 0.7 to 0.9132260271183847
  static const double aa3=0.9132260271183847;
  static const int ncheb2=27;
  static const double cheb2[] =
  {
    1.1139496701107756,0.3523967429328067,0.0713849171926623,0.01475818043595387,0.003381255637322462,
    0.0008228057599452224,0.00020785506681254216,0.00005390169253706556,0.000014250571923902464,3.823880733161044e-6,
    1.0381966089136036e-6,2.8457557457837253e-7,7.86223332179956e-8,2.1866609342508474e-8,6.116186259857143e-9,
    1.7191233618437565e-9,4.852755117740807e-10,1.3749966961763457e-10,3.908961987062447e-11,1.1146253766895824e-11,
    3.1868887323415814e-12,9.134319791300977e-13,2.6211077371181566e-13,7.588643377757906e-14,2.1528376972619e-14,
    6.030906040404772e-15,1.9549163926819867e-15};
  // Chebyshev with exp/log  scale
  // a = -Log[1 - SynFracInt[1]]; b = -Log[1 - SynFracInt[7]];
  static const double aa4=2.4444485538746025480,aa5=9.3830728608909477079;
  static const int ncheb3=28;
  static const double cheb3[] =
  {
    1.2292683840435586977,0.160353449247864455879,-0.0353559911947559448721,0.00776901561223573936985,
    -0.00165886451971685133259,0.000335719118906954279467,-0.0000617184951079161143187,9.23534039743246708256e-6,
    -6.06747198795168022842e-7,-3.07934045961999778094e-7,1.98818772614682367781e-7,-8.13909971567720135413e-8,
    2.84298174969641838618e-8,-9.12829766621316063548e-9,2.77713868004820551077e-9,-8.13032767247834023165e-10,
    2.31128525568385247392e-10,-6.41796873254200220876e-11,1.74815310473323361543e-11,-4.68653536933392363045e-12,
    1.24016595805520752748e-12,-3.24839432979935522159e-13,8.44601465226513952994e-14,-2.18647276044246803998e-14,
    5.65407548745690689978e-15,-1.46553625917463067508e-15,3.82059606377570462276e-16,-1.00457896653436912508e-16};
  static const double aa6=33.122936966163038145;
  static const int ncheb4=27;
  static const double cheb4[] =
  {
    1.69342658227676741765,0.0742766400841232319225,-0.019337880608635717358,0.00516065527473364110491,
    -0.00139342012990307729473,0.000378549864052022522193,-0.000103167085583785340215,0.0000281543441271412178337,
    -7.68409742018258198651e-6,2.09543221890204537392e-6,-5.70493140367526282946e-7,1.54961164548564906446e-7,
    -4.19665599629607704794e-8,1.13239680054166507038e-8,-3.04223563379021441863e-9,8.13073745977562957997e-10,
    -2.15969415476814981374e-10,5.69472105972525594811e-11,-1.48844799572430829499e-11,3.84901514438304484973e-12,
    -9.82222575944247161834e-13,2.46468329208292208183e-13,-6.04953826265982691612e-14,1.44055805710671611984e-14,
    -3.28200813577388740722e-15,6.96566359173765367675e-16,-1.294122794852896275e-16};

  if(x<aa2)      return x*x*x*Chebyshev(aa1,aa2,cheb1,ncheb1,x);
  else if(x<aa3) return       Chebyshev(aa2,aa3,cheb2,ncheb2,x);
  else if(x<1-0.0000841363)
  {
    double y=-log(1-x);
    return y*Chebyshev(aa4,aa5,cheb3,ncheb3,y);
  }
  else
  {
    double y=-log(1-x);
    return y*Chebyshev(aa5,aa6,cheb4,ncheb4,y);
  }
}
