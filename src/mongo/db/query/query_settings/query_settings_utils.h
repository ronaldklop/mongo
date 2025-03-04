/**
 *    Copyright (C) 2023-present MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the Server Side Public License, version 1,
 *    as published by MongoDB, Inc.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Server Side Public License for more details.
 *
 *    You should have received a copy of the Server Side Public License
 *    along with this program. If not, see
 *    <http://www.mongodb.com/licensing/server-side-public-license>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the Server Side Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#pragma once

#include "mongo/db/curop.h"
#include "mongo/db/query/query_settings/query_settings_gen.h"
#include "mongo/db/query/query_settings/query_settings_manager.h"
#include "mongo/db/query/query_shape/agg_cmd_shape.h"
#include "mongo/db/query/query_shape/distinct_cmd_shape.h"
#include "mongo/db/query/query_shape/find_cmd_shape.h"
#include "mongo/db/query/query_shape/query_shape.h"
#include "mongo/stdx/unordered_set.h"

namespace mongo::query_settings {

struct RepresentativeQueryInfo {
    const BSONObj serializedQueryShape;
    const query_shape::QueryShapeHash queryShapeHash;
    const NamespaceString namespaceString;
    const stdx::unordered_set<NamespaceString> involvedNamespaces;
    const boost::optional<mongo::EncryptionInformation> encryptionInformation;
    const bool isIdHackQuery;
    // Name of the leading stage if it is "system"/"administrative" and is not eligible for
    // rejection by query settings.
    const boost::optional<std::string> systemStage;
};

/**
 * Creates the corresponding RepresentativeQueryInfo instance by parsing the representative
 * query BSONObj.
 */
RepresentativeQueryInfo createRepresentativeInfo(OperationContext* opCtx,
                                                 const BSONObj& cmd,
                                                 const boost::optional<TenantId>& tenantId);

QuerySettings lookupQuerySettingsForFind(const boost::intrusive_ptr<ExpressionContext>& expCtx,
                                         const ParsedFindCommand& parsedFind,
                                         const NamespaceString& nss);

QuerySettings lookupQuerySettingsForAgg(
    const boost::intrusive_ptr<ExpressionContext>& expCtx,
    const AggregateCommandRequest& aggregateCommandRequest,
    const Pipeline& pipeline,
    const stdx::unordered_set<NamespaceString>& involvedNamespaces,
    const NamespaceString& nss);

QuerySettings lookupQuerySettingsForDistinct(const boost::intrusive_ptr<ExpressionContext>& expCtx,
                                             const ParsedDistinctCommand& parsedDistinct,
                                             const NamespaceString& nss);

namespace utils {

/**
 * Determines if 'querySettings' field is allowed to be present as part of the command request for
 * the given 'client'.
 */
bool allowQuerySettingsFromClient(Client* client);

/**
 * Returns true if a given QuerySettings instance contains only default values.
 */
bool isDefault(const QuerySettings& settings);

void validateRepresentativeQuery(const RepresentativeQueryInfo& representativeQueryInfo);
void validateQuerySettings(const QuerySettings& settings);
void verifyQueryCompatibleWithSettings(const RepresentativeQueryInfo& representativeQueryInfo,
                                       const QuerySettings& settings);
void simplifyQuerySettings(QuerySettings& settings);
void sanitizeQuerySettingsHints(std::vector<QueryShapeConfiguration>& queryShapeConfigs);
}  // namespace utils
}  // namespace mongo::query_settings
