#ifndef __APP_DELEGATE_H__
#define __APP_DELEGATE_H__

#include "cobins.h"
#include <string>
USING_NS_COB;
USING_NS_LL;

class AppDelegate : public cobins::Application
{
public:
    AppDelegate();
    bool create(const std::string& name, Context& context = Context::DEFAULT);
    bool create(HWND hWnd, const std::string& name, Context& context = Context::DEFAULT);

protected:
    virtual void onCreate(const lianli::Context& context) override;
    virtual void onStart() override;
    virtual bool onEventProc(const std::string& evtName, EvtData& evtData) override;
    virtual void onStop() override;
    virtual void onDestroy(const lianli::Context& context) override;

protected:
public:
    cobins::Bin mBin;

    DECLARE_TRANS_TABLE()
};

#endif
