// Copyright 2010-2015 RethinkDB, all rights reserved.
#ifndef CLUSTERING_ADMINISTRATION_AUTH_PERMISSIONS_ARTIFICIAL_TABLE_BACKEND_HPP
#define CLUSTERING_ADMINISTRATION_AUTH_PERMISSIONS_ARTIFICIAL_TABLE_BACKEND_HPP

#include "errors.hpp"
#include <boost/optional.hpp>

#include "clustering/administration/auth/base_artificial_table_backend.hpp"

class name_resolver_t;

namespace auth {

class permissions_artificial_table_backend_t :
    public base_artificial_table_backend_t
{
public:
    permissions_artificial_table_backend_t(
        rdb_context_t *rdb_context,
        lifetime_t<name_resolver_t const &> name_resolver,
        std::shared_ptr<semilattice_readwrite_view_t<auth_semilattice_metadata_t>>
            auth_semilattice_view,
        std::shared_ptr<semilattice_read_view_t<cluster_semilattice_metadata_t>>
            cluster_semilattice_view,
        admin_identifier_format_t identifier_format);

    bool read_all_rows_as_vector(
        auth::user_context_t const &user_context,
        signal_t *interruptor,
        std::vector<ql::datum_t> *rows_out,
        admin_err_t *error_out);

    bool read_row(
        auth::user_context_t const &user_context,
        ql::datum_t primary_key,
        signal_t *interruptor,
        ql::datum_t *row_out,
        admin_err_t *error_out);

    bool write_row(
        auth::user_context_t const &user_context,
        ql::datum_t primary_key,
        bool pkey_was_autogenerated,
        ql::datum_t *new_value_inout,
        UNUSED signal_t *interruptor,
        admin_err_t *error_out);

private:
    uint8_t parse_primary_key(
        ql::datum_t const &primary_key,
        cluster_semilattice_metadata_t const &cluster_metadata,
        username_t *username_out,
        database_id_t *database_id_out,
        namespace_id_t *table_id_out,
        admin_err_t *admin_err_out = nullptr);

    bool global_to_datum(
        username_t const &username,
        permissions_t const &permissions,
        ql::datum_t *datum_out);

    bool database_to_datum(
        username_t const &username,
        database_id_t const &database_id,
        permissions_t const &permissions,
        cluster_semilattice_metadata_t const &cluster_metadata,
        ql::datum_t *datum_out);

    bool table_to_datum(
        username_t const &username,
        boost::optional<database_id_t> const &database_id,
        namespace_id_t const &table_id,
        permissions_t const &permissions,
        cluster_semilattice_metadata_t const &cluster_metadata,
        ql::datum_t *datum_out);

private:
    name_resolver_t const &m_name_resolver;
    admin_identifier_format_t m_identifier_format;
};

}  // namespace auth

#endif  // CLUSTERING_ADMINISTRATION_AUTH_PERMISSIONS_ARTIFICIAL_TABLE_BACKEND_HPP