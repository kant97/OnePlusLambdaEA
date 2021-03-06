#include "resultsTablePrinter.h"
#include <iostream>
#include <iomanip>
#include "logger.h"
#include <cassert>
#include <iterator>

ResultsTablePrinter::ResultsTablePrinter(const std::string &fileName, uint32_t offset,
                           const ParsedParams &params) : ResultsTable(fileName),
    toPrintNextLambda(params.lambda[0]), toPrintNextN(params.n[0]),
    fout(fileName), offset(offset),
    N_BEGIN(params.n[0]), N_END(params.n[1]), N_STEP(params.n[2]),
    LAMBDA_BEGIN(params.lambda[0]), LAMBDA_END(params.lambda[1]), LAMBDA_STEP(params.lambda[2])
{
    fout << std::setw(offset) << "lambda" << std::setw(offset) << "n" << std::setw(
             offset) << "function evaluations" << std::setw(offset) << "standard deviation" << std::endl;
    fout << std::endl;
}

void ResultsTablePrinter::add(uint lambda, uint64_t n, uint evoluations, double deviation)
{
    data_mtx.lock();
    data[lambda][n] = std::make_pair(evoluations, deviation);
    LOG("Done for lambda =", lambda, ", n =", n, ", for file:", fileName);
    data_mtx.unlock();
}

void ResultsTablePrinter::printResults()
{
    data_mtx.lock();
    for (uint &i = toPrintNextLambda; i <= LAMBDA_END; i += LAMBDA_STEP)
    {
        if (toPrintNextN > N_END) toPrintNextN = N_BEGIN;
        if (!data.count(i)) break;
        bool ok = false;
        for (uint &j = toPrintNextN; j <= N_END; j += N_STEP)
        {
            ok = false;
            if (!data[i].count(j)) break;
            ok = true;
            print(i, j, data[i][j].first, data[i][j].second);
        }
        if (!ok) break;
        print();
    }
    data_mtx.unlock();
}

void ResultsTablePrinter::print(uint32_t lambda, uint32_t n, uint32_t ans, double deviation)
{
    fout << std::setw(offset) << lambda << std::setw(offset) << n << std::setw(
             offset) << ans << std::setw(offset) << deviation << std::endl;
}

void ResultsTablePrinter::print()
{
    fout << std::endl;
}
