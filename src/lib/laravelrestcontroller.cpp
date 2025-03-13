#include "laravelrestcontroller.h"

#include <DataGate/dataquery.h>
#include <DataGate/dataresponse.h>

#include <Jsoner/object.h>
#include <Jsoner/array.h>

#include <RestLink/request.h>
#include <RestLink/body.h>
#include <RestLink/response.h>

#include <QtCore/qvariant.h>

LaravelRestController::LaravelRestController(RestLink::Api *api)
    : m_api(api)
{
}

LaravelRestController::~LaravelRestController()
{
}

void LaravelRestController::fetchSomeSearchSuggestions(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse)
{
    RestLink::Request request(endpoint(query) + "/search/suggestions");
    request.addQueryParameter("query", query.string());

    auto processing = [=](RestLink::Response *response) {
        DataGate::DataResponse res;
        res.setSuccess(response->isSuccess());
        res.setArray(response->readJsonArray());
        onResponse(res);
    };

    RestLink::Response *response = m_api->get(request);
    registerResponse(response, processing, onProgress);
}

void LaravelRestController::fetchManyObjects(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse)
{
    RestLink::Request request(endpoint(query));
    request.addQueryParameter("query", query.string());

    auto processing = [=](RestLink::Response *response) {
        const Jsoner::Object object = response->readJsonObject();

        DataGate::DataResponse res;
        res.setSuccess(response->isSuccess());
        res.setArray(object.array("data"));
        res.setPage(object.integer("current_page"));
        res.setPageCount(object.integer("last_page"));
        onResponse(res);
    };

    RestLink::Response *response = m_api->get(request);
    registerResponse(response, processing, onProgress);
}

void LaravelRestController::fetchOneObject(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse)
{
    RestLink::Request request(endpoint(query));
    request.addQueryParameter("query", query.string());

    auto processing = [=](RestLink::Response *response) {
        DataGate::DataResponse res;
        res.setSuccess(response->isSuccess());
        res.setObject(response->readJsonObject());
        onResponse(res);
    };

    RestLink::Response *response = m_api->get(request);
    registerResponse(response, processing, onProgress);
}

void LaravelRestController::addOneObject(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse)
{
    RestLink::Request request(endpoint(query));
    request.addQueryParameter("query", query.string());

    auto processing = [=](RestLink::Response *response) {
        DataGate::DataResponse res;
        res.setSuccess(response->isSuccess());
        res.setObject(response->readJsonObject());
        onResponse(res);
    };

    RestLink::Response *response = m_api->post(request, query.object());
    registerResponse(response, processing, onProgress);
}

void LaravelRestController::editOneObject(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse)
{
    RestLink::Request request(endpoint(query) + '/' + query.object().string("id"));
    request.addQueryParameter("query", query.string());

    auto processing = [=](RestLink::Response *response) {
        DataGate::DataResponse res;
        res.setSuccess(response->isSuccess());
        res.setObject(response->readJsonObject());
        onResponse(res);
    };

    RestLink::Response *response = m_api->put(request, query.object());
    registerResponse(response, processing, onProgress);
}

void LaravelRestController::deleteManyObjects(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse)
{
    bool single = query.array().count() == 1;

    RestLink::Request request(endpoint(query) + '/' + (single ? query.object().string("id") : "bulk-delete"));
    request.addQueryParameter("query", query.string());

    auto processing = [=](RestLink::Response *response) {
        DataGate::DataResponse res;
        res.setSuccess(response->isSuccess());
        res.setObject(response->readJsonObject());
        onResponse(res);
    };

    RestLink::Response *response;

    if (single) {
        response = m_api->deleteResource(request);
    } else {
        Jsoner::Array ids;
        Jsoner::Array objects = query.array();
        std::transform(objects.begin(), objects.end(), std::back_inserter(ids), [](const QJsonValue &value) {
            return value.toObject().value("id");
        });
        response = m_api->post(request, ids);
    }

    registerResponse(response, processing, onProgress);
}

QString LaravelRestController::endpoint(const DataGate::DataQuery &query)
{
    return query.parameter("endpoint").toString();
}

void LaravelRestController::registerResponse(RestLink::Response *response, const RestLink::ApiRunCallback &callback, const DataGate::DataQueryProgressCallback &onProgress)
{
    QObject::connect(response, &RestLink::Response::downloadProgress, response, onProgress);
    QObject::connect(response, &RestLink::Response::finished, response, [callback, response] { callback(response); });
    QObject::connect(response, &RestLink::Response::finished, response, &QObject::deleteLater);
}
