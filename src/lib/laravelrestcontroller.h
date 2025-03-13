#ifndef LARAVELRESTCONTROLLER_H
#define LARAVELRESTCONTROLLER_H

#include <LaravelGate/global.h>

#include <DataGate/abstractdatacontroller.h>

#include <RestLink/api.h>

class LARAVELGATE_EXPORT LaravelRestController : public DataGate::AbstractDataController
{
public:
    explicit LaravelRestController(RestLink::Api *api);
    virtual ~LaravelRestController();

    void fetchSomeSearchSuggestions(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse) override;
    void fetchManyObjects(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse) override;
    void fetchOneObject(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse) override;
    void addOneObject(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse) override;
    void editOneObject(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse) override;
    void deleteManyObjects(const DataGate::DataQuery &query, const DataGate::DataQueryProgressCallback &onProgress, const DataGate::DataQueryResponseCallback &onResponse) override;

    static QString endpoint(const DataGate::DataQuery &query);
    //static int objectId(const Jsoner::Object &object);

private:
    static void registerResponse(RestLink::Response *response, const std::function<RestLink::ApiRunCallback> &callback, const DataGate::DataQueryProgressCallback &onProgress);
    RestLink::Api *m_api;
};

#endif // LARAVELRESTCONTROLLER_H
