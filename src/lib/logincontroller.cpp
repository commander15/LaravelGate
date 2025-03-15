#include "logincontroller.h"
#include "logincontroller_p.h"

#include <LaravelGate/datacontroller.h>

#include <DataGate/authenticator.h>
#include <DataGate/dataquery.h>
#include <DataGate/dataresponse.h>

#include <Jsoner/object.h>
#include <Jsoner/array.h>

#include <RestLink/request.h>
#include <RestLink/body.h>
#include <RestLink/response.h>

#include <QtCore/qvariant.h>

namespace LaravelGate {

LoginController::LoginController(RestLink::Api *api)
    : m_api(api)
{
}

void LoginController::attemptLogIn(const DataGate::LoginQuery &query, const DataGate::DataQueryResponseCallback &callback)
{
    RestLink::Request request("/users/login");

    Jsoner::Object object;
    object.put("email", query.identifier());
    object.put("password", query.password());

    m_api->post(request, object, [=](RestLink::Response *response) {
        DataGate::DataResponse res;
        res.setSuccess(response->isSuccess());

        if (response->isSuccess()) {
            res.setObject(response->readJsonObject());
        } else {
            res.setCode(DataGate::AuthenticationError::BadCredentials);
        }

        callback(res);
    });
}

void LoginController::attemptLogOut(const DataGate::LoginQuery &query, const DataGate::DataQueryResponseCallback &callback)
{
    RestLink::Request request("/users/logout");

    const QVariant token = query.parameter("token");
    if (!token.isNull())
        request.setHeader("Authorization", "Bearer " + token.toString());

    m_api->post(request, query.object(), [=](RestLink::Response *response) {
        DataGate::DataResponse res;
        res.setSuccess(response->isSuccess());

        if (response->isSuccess()) {
            res.setObject(response->readJsonObject());
        } else {
            res.setCode(DataGate::AuthenticationError::BadCredentials);
        }

        callback(res);
    });
}

} // namespace LaravelGate
