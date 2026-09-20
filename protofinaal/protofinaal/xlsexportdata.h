#ifndef XLSEXPORTDATA_H
#define XLSEXPORTDATA_H

#include <QVector>

#include "xlsexportservice.h"

class TeamsTable;

struct XlsExportData {
    int maxShots = 0;
    QVector<XlsTeamBlock> blocks;
};

XlsExportData buildXlsExportData(const QVector<TeamsTable*> &tables);

// Rows to write to the sheet for a team block: the team total row, plus each
// competitor's row unless there is only one competitor, whose row would just
// duplicate the team total row.
QVector<XlsShotRow> collectXlsRowsForBlock(const XlsTeamBlock &block);

#endif // XLSEXPORTDATA_H
