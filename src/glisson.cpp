
#include <flext.h>
#include <random>
#include <cmath>

#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 400)
#error You need at least flext version 0.4.0
#endif

class glisson: public flext_dsp{

	FLEXT_HEADER(glisson,flext_dsp)
 
public:
	glisson();

protected:
    virtual void m_signal(int n, float *const *in, float *const *out);

    void m_start_low(float);
    void m_start_high(float);
    void m_end_low(float);
    void m_end_high(float);

private:

    float random(float min, float max);
    void trigger();

    FLEXT_CALLBACK_F(m_start_low);
    FLEXT_CALLBACK_F(m_start_high);
    FLEXT_CALLBACK_F(m_end_low);
    FLEXT_CALLBACK_F(m_end_high);

    float mPreviousSample;
    float mStart;
    float mDistance;

    float mStartLow;
    float mStartHigh;
    float mEndLow;
    float mEndHigh;

    std::default_random_engine re;

};

FLEXT_NEW_DSP("glisson~",glisson)

glisson::glisson():
mPreviousSample(1.0){

	AddInAnything("phase"); 
    AddOutSignal("glisson");

    FLEXT_ADDMETHOD_F(0,"start_low", m_start_low);
    FLEXT_ADDMETHOD_F(0,"start_high", m_start_high); 
    FLEXT_ADDMETHOD_F(0,"end_low", m_end_low); 
    FLEXT_ADDMETHOD_F(0,"end_high", m_end_high); 
} 

void glisson::m_start_low(float value){ mStartLow = value;}
void glisson::m_start_high(float value){ mStartHigh = value;}
void glisson::m_end_low(float value){ mEndLow = value;}
void glisson::m_end_high(float value){ mEndHigh = value;}

void glisson::m_signal(int n, float *const *in, float *const *out){

    float *ins = in[0];
    float *outs = out[0];

    while(n--){
    	float currentSample = *ins++;
    	if( currentSample < mPreviousSample){
    		// down
    		trigger();
    	}

    	*outs++ = currentSample * mDistance + mStart;
    	mPreviousSample = currentSample;
    }
} 

void glisson::trigger(){
	mStart = random(mStartLow, mStartHigh);
	mDistance = random(mEndLow, mEndHigh) - mStart;
}

float glisson::random(float min, float max){
    std::uniform_real_distribution<float> unif(min, max);
    return unif(re);
}