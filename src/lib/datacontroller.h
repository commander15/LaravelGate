#ifndef LARAVELGATE_DATACONTROLLER_H
#define LARAVELGATE_DATACONTROLLER_H

#include <LaravelGate/global.h>

#include <DataGate/abstractdatacontroller.h>

#include <RestLink/api.h>

#include <QtCore/qcoreapplication.h>

namespace LaravelGate {

class LARAVELGATE_EXPORT DataController : public DataGate::AbstractDataController
{
    Q_DECLARE_TR_FUNCTIONS(DataController)

public:
    explicit DataController(RestLink::Api *api);
    virtual ~DataController() = default;

    static void registerResponse(RestLink::Response *response, const RestLink::ApiRunCallback &callback, const DataGate::DataQueryProgressCallback &onProgress);

protected:
    void fetchSomeSearchSuggestions(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse) override;
    void fetchManyObjects(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse) override;
    void fetchOneObject(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse) override;
    void addOneObject(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse) override;
    void editOneObject(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse) override;
    void deleteManyObjects(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse) override;

    static QString endpoint(const DataGate::DataQuery &query);
    //static int objectId(const Jsoner::Object &object);

private:
    RestLink::Api *m_api;
};

}

#endif // LARAVELGATE_DATACONTROLLER_H
