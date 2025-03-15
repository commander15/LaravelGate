#ifndef LARAVELGATE_LOGINCONTROLLER_H
#define LARAVELGATE_LOGINCONTROLLER_H

#include <LaravelGate/global.h>

#include <DataGate/abstractlogincontroller.h>

#include <RestLink/api.h>

namespace LaravelGate {

class LARAVELGATE_EXPORT LoginController : public DataGate::AbstractLoginController
{
public:
    explicit LoginController(RestLink::Api *api);
    virtual ~LoginController() = default;

protected:
    void attemptLogIn(const DataGate::LoginQuery &query, const DataGate::DataQueryResponseCallback &callback) override;
    void attemptLogOut(const DataGate::LoginQuery &query, const DataGate::DataQueryResponseCallback &callback) override;

private:
    RestLink::Api *m_api;
};

} // namespace LaravelGate

#endif // LARAVELGATE_LOGINCONTROLLER_H
