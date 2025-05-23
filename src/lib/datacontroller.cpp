#include "datacontroller.h"

#include <DataGate/dataquery.h>
#include <DataGate/dataresponse.h>

#include <Jsoner/object.h>
#include <Jsoner/array.h>

#include <RestLink/request.h>
#include <RestLink/body.h>
#include <RestLink/response.h>

#include <QtCore/qvariant.h>

namespace LaravelGate {

DataController::DataController(RestLink::Api *api)
    : m_api(api)
{
}

void DataController::registerResponse(RestLink::Response *response, const RestLink::ApiRunCallback &callback, const DataGate::DataQueryProgressCallback &onProgress)
{
    QObject::connect(response, &RestLink::Response::downloadProgress, response, onProgress);
    QObject::connect(response, &RestLink::Response::finished, response, [callback, response] { callback(response); });
    QObject::connect(response, &RestLink::Response::finished, response, &QObject::deleteLater);
}

void DataController::fetchSomeSearchSuggestions(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse)
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

void DataController::fetchManyObjects(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse)
{
    RestLink::Request request(endpoint(query));

    const QString queryString = query.string();
    if (queryString.size() > 0)
        request.addQueryParameter("query", queryString);

    const QVariantHash filters = query.filters();
    const QStringList filterNames = filters.keys();
    for (const QString &filterName : filterNames)
        request.addQueryParameter(filterName, filters.value(filterName));

    const QString sortField = query.sortField();
    if (!sortField.isEmpty())
        request.addQueryParameter("sort_by", sortField + '.' + (query.sortOrder() == Qt::DescendingOrder ? "desc" : "asc"));

    request.addQueryParameter("page", query.page());

    auto processing = [=](RestLink::Response *response) {
        DataGate::DataResponse res;
        res.setSuccess(response->isSuccess());

        if (response->isSuccess()) {
            const Jsoner::Object object = response->readJsonObject();
            res.setArray(object.array("data"));
            res.setPage(object.integer("current_page"));
            res.setPageCount(object.integer("last_page"));

            const QStringList fields = { "from", "to", "per_page", "total" };
            for (const QString &field : fields)
                res.setData(field, object.variant(field));
        } else if (response->hasHttpStatusCode()) {
            const Jsoner::Object object = response->readJsonObject();

            res.setTitle(tr("Error"));
            res.setText(tr("The server reported an error."));
            if (object.has("message"))
                res.setInformativeText(object.string("message"));

            if (object.contains("errors")) {
                QString msg;
                const Jsoner::Array errors = object.array("errors");

                for (const QJsonValue &value : errors)
                    msg.append(value.toString() + "\n");

                msg.append(QStringLiteral("\nStatus: HTTP %1 %2").arg(response->httpStatusCode()).arg(response->httpReasonPhrase()));
                msg.append("\n*" + tr("If the problem persists, please contact the developer."));

                res.setDetailedText(msg);
            }

            res.setObject(object);
        } else if (response->hasNetworkError()) {
            res.setTitle(tr("Network Error"));
            res.setText(tr("A network error occured"));
            res.setDetailedText(response->networkErrorString());
        } else {
            res.setTitle(tr("Error"));
            res.setText(tr("An unknown error occured, we don't known more"));
        }

        onResponse(res);
    };

    RestLink::Response *response = m_api->get(request);
    registerResponse(response, processing, onProgress);
}

void DataController::fetchOneObject(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse)
{
    RestLink::Request request(endpoint(query) + '/' + query.object().string("id"));

    auto processing = [=](RestLink::Response *response) {
        DataGate::DataResponse res;
        res.setSuccess(response->isSuccess());
        res.setObject(response->readJsonObject());
        onResponse(res);
    };

    RestLink::Response *response = m_api->get(request);
    registerResponse(response, processing, onProgress);
}

void DataController::addOneObject(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse)
{
    RestLink::Request request(endpoint(query));

    auto processing = [=](RestLink::Response *response) {
        DataGate::DataResponse res;
        res.setSuccess(response->isSuccess());
        res.setObject(response->readJsonObject());
        onResponse(res);
    };

    RestLink::Response *response = m_api->post(request, query.object());
    registerResponse(response, processing, onProgress);
}

void DataController::editOneObject(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse)
{
    RestLink::Request request(endpoint(query) + '/' + query.object().string("id"));

    auto processing = [=](RestLink::Response *response) {
        DataGate::DataResponse res;
        res.setSuccess(response->isSuccess());
        res.setObject(response->readJsonObject());
        onResponse(res);
    };

    RestLink::Response *response = m_api->put(request, query.object());
    registerResponse(response, processing, onProgress);
}

void DataController::deleteManyObjects(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse)
{
    bool single = query.array().count() == 1;

    RestLink::Request request(endpoint(query) + '/' + (single ? query.object().string("id") : "bulk-delete"));

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

QString DataController::endpoint(const DataGate::DataQuery &query)
{
    return query.parameter("endpoint").toString();
}

}
