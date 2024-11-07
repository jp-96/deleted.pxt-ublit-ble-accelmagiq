#include "pxt.h"
#include "lib/AccelMagiQService.h"

namespace custom
{
    AccelMagiQService *_pService = NULL;

    //%
    void startService()
    {
        if (NULL != _pService)
            return;
        _pService = new AccelMagiQService();
    }

    //%
    void notifyData(int w10000, int x10000, int y10000, int z10000)
    {
        startService();
        _pService->quaternionUpdate(w10000, x10000, y10000, z10000);
    }

}
