// @HEADER
// ***********************************************************************
//
//          Tpetra: Templated Linear Algebra Services Package
//                 Copyright (2008) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Michael A. Heroux (maherou@sandia.gov)
//
// ************************************************************************
// @HEADER

#ifndef TPETRA_IMPORT_DECL_HPP
#define TPETRA_IMPORT_DECL_HPP

#include <Tpetra_Details_Transfer.hpp>

namespace Tpetra {
  //
  // Forward declarations.  The "doxygen" bit simply tells Doxygen
  // (our automatic documentation generation system) to skip forward
  // declarations.
  //
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  template<class LocalOrdinal, class GlobalOrdinal, class Node>
  class ImportExportData;

  template<class LocalOrdinal, class GlobalOrdinal, class Node>
  class Export;
#endif // DOXYGEN_SHOULD_SKIP_THIS

  /// \brief Communication plan for data redistribution from a
  ///   uniquely-owned to a (possibly) multiply-owned distribution.
  ///
  /// \tparam LocalOrdinal The type of local indices.  See the
  ///   documentation of Map for requirements.
  /// \tparam GlobalOrdinal The type of global indices.  See the
  ///   documentation of Map for requirements.
  /// \tparam Node The Kokkos Node type.  See the documentation of Map
  ///   for requirements.
  ///
  /// Tpetra users should use this class to construct a communication
  /// plan between two data distributions (i.e., two Map objects).
  /// The plan can be called repeatedly by computational classes to
  /// perform communication according to the same pattern.
  /// Constructing the plan may be expensive, both in terms of
  /// communication and computation.  However, it can be reused
  /// inexpensively.
  ///
  /// Tpetra has two classes for data redistribution: Import and
  /// Export.  Import is for redistributing data from a uniquely-owned
  /// distribution to a possibly multiply-owned distribution.  Export
  /// is for redistributing data from a possibly multiply-owned
  /// distribution to a uniquely-owned distribution.
  ///
  /// The names "Import" and "Export" have nothing to do with the
  /// direction in which data moves relative to the calling process;
  /// any process may do both receives and sends in an Import or
  /// Export.  Rather, the names suggest what happens in their most
  /// common use case, the communication pattern for sparse
  /// matrix-vector multiply.  Import "brings in" remote source vector
  /// data (from the domain Map to the column Map) for local
  /// computation, and Export "pushes" the result back (from the row
  /// Map to the range Map).  Import and Export have other uses as
  /// well.
  ///
  /// As mentioned above, one use case of Import is bringing in remote
  /// source vector data for a distributed sparse matrix-vector
  /// multiply.  The source vector itself is uniquely owned, but must
  /// be brought in into an overlapping distribution so that each
  /// process can compute its part of the target vector without
  /// further communication.
  ///
  /// Epetra separated Import and Export for performance reasons.  The
  /// implementation is different, depending on which direction is the
  /// uniquely-owned Map.  Tpetra retains this convention.
  ///
  /// This class is templated on the same template arguments as Map:
  /// the local ordinal type <tt>LocalOrdinal</tt>, the global ordinal
  /// type <tt>GlobalOrdinal</tt>, and the Kokkos <tt>Node</tt> type.
  ///
  /// This method accepts an optional list of parameters, either
  /// through the constructor or through the setParameterList()
  /// method.  Most users do not need to worry about these parameters;
  /// the default values are fine.  However, for expert users, we
  /// expose the following parameter:
  /// - "Barrier between receives and sends" (\c bool): Whether to
  ///   execute a barrier between receives and sends, when executing
  ///   the Import (i.e., when calling DistObject's doImport()
  ///   (forward mode) or doExport() (reverse mode)).
  ///
  template<class LocalOrdinal = ::Tpetra::Details::DefaultTypes::local_ordinal_type,
           class GlobalOrdinal = ::Tpetra::Details::DefaultTypes::global_ordinal_type,
           class Node = ::Tpetra::Details::DefaultTypes::node_type>
  class Import:
    public ::Tpetra::Details::Transfer<LocalOrdinal, GlobalOrdinal, Node>
  {
  private:
    friend class Export<LocalOrdinal,GlobalOrdinal,Node>;
  public:
    //! The specialization of Map used by this class.
    //typedef typename ::Tpetra::Details::Transfer<LocalOrdinal, GlobalOrdinal, Node>::map_type map_type;
    typedef ::Tpetra::Map<LocalOrdinal, GlobalOrdinal, Node> map_type;

    //! @name Constructor/Destructor Methods
    //@{

    /// \brief Construct an Import from the source and target Maps.
    ///
    /// \param source [in] The source distribution.  This <i>must</i>
    ///   be a uniquely owned (nonoverlapping) distribution.
    ///
    /// \param target [in] The target distribution.  This may be a
    ///   multiply owned (overlapping) distribution.
    Import (const Teuchos::RCP<const map_type>& source,
            const Teuchos::RCP<const map_type>& target);

    /// \brief Construct an Import from the source and target Maps,
    ///   with an output stream for debugging output.
    ///
    /// \param source [in] The source distribution.  This <i>must</i>
    ///   be a uniquely owned (nonoverlapping) distribution.
    ///
    /// \param target [in] The target distribution.  This may be a
    ///   multiply owned (overlapping) distribution.
    ///
    /// \param out [in/out] Output stream for debugging output.
    Import (const Teuchos::RCP<const map_type>& source,
            const Teuchos::RCP<const map_type>& target,
            const Teuchos::RCP<Teuchos::FancyOStream>& out);

    /// \brief Constructor (with list of parameters)
    ///
    /// \param source [in] The source distribution.  This <i>must</i>
    ///   be a uniquely owned (nonoverlapping) distribution.
    ///
    /// \param target [in] The target distribution.  This may be a
    ///   multiply owned (overlapping) distribution.
    ///
    /// \param plist [in/out] List of parameters.  Currently passed
    ///   directly to the Distributor that implements communication.
    ///   If you don't know what this should be, you should use the
    ///   two-argument constructor, listed above.
    Import (const Teuchos::RCP<const map_type>& source,
            const Teuchos::RCP<const map_type>& target,
            const Teuchos::RCP<Teuchos::ParameterList>& plist);

    /// \brief Constructor (with list of parameters and debug output stream)
    ///
    /// \param source [in] The source distribution.  This <i>must</i>
    ///   be a uniquely owned (nonoverlapping) distribution.
    ///
    /// \param target [in] The target distribution.  This may be a
    ///   multiply owned (overlapping) distribution.
    ///
    /// \param out [in/out] Output stream (for printing copious debug
    ///   output on all processes, if that option is enabled).
    ///
    /// \param plist [in/out] List of parameters.  Currently passed
    ///   directly to the Distributor that implements communication.
    ///   If you don't know what this should be, you should use the
    ///   two-argument constructor, listed above.
    Import (const Teuchos::RCP<const map_type>& source,
            const Teuchos::RCP<const map_type>& target,
            const Teuchos::RCP<Teuchos::FancyOStream>& out,
            const Teuchos::RCP<Teuchos::ParameterList>& plist);

    /// \brief Construct an Import from the source and target Maps.
    ///
    /// \param source [in] The source distribution.  This <i>must</i>
    ///   be a uniquely owned (nonoverlapping) distribution.
    ///
    /// \param target [in] The target distribution.  This may be a
    ///   multiply owned (overlapping) distribution.
    ///
    /// \param remotePIDs [in] Owning PIDs corresponding to the remoteGIDs.
    /// If this information is available one can reduce the cost of the Import
    /// constructor.
    Import (const Teuchos::RCP<const map_type>& source,
            const Teuchos::RCP<const map_type>& target,
            Teuchos::Array<int> & remotePIDs,
	    const Teuchos::RCP<Teuchos::ParameterList>& plist = Teuchos::rcp(new Teuchos::ParameterList) );

    /// \brief Copy constructor.
    ///
    /// \note Currently this only makes a shallow copy of the Import's
    ///   underlying data.
    Import (const Import<LocalOrdinal,GlobalOrdinal,Node>& importer);

    /// \brief "Copy" constructor from an Export object.
    ///
    /// This constructor creates an Import object from the "reverse"
    /// of the given Export object.  This method is mainly useful for
    /// Tpetra developers, for example when building the explicit
    /// transpose of a sparse matrix.
    Import (const Export<LocalOrdinal,GlobalOrdinal,Node>& exporter);

    /// \bief Full Expert constructor
    /// Requirements: source and target maps are fully correct
    ///
    // cblcbl this is the guy to call for reverse comm....
    Import (const Teuchos::RCP<const Map<LocalOrdinal, GlobalOrdinal, Node> >& source,
            const Teuchos::RCP<const Map<LocalOrdinal, GlobalOrdinal, Node> >& target,
            const Teuchos::ArrayView<int> & remotePIDs,
            const Teuchos::ArrayView<const LocalOrdinal> & userExportLIDs,
            const Teuchos::ArrayView<const int> & userExportPIDs,
            const Teuchos::RCP<Teuchos::ParameterList>& plist = Teuchos::null,
            const Teuchos::RCP<Teuchos::FancyOStream>& out = Teuchos::null);


    //! Destructor.
    virtual ~Import ();

    /// \brief Set parameters.
    ///
    /// Please see the class documentation for a list of all accepted
    /// parameters and their default values.
    void setParameterList (const Teuchos::RCP<Teuchos::ParameterList>& plist);

    //@}
    //! @name Import Attribute Methods
    //@{

    /// \brief Number of initial identical IDs.
    ///
    /// The number of IDs that are identical between the source and
    /// target Maps, up to the first different ID.
    size_t getNumSameIDs() const;

    /// \brief Number of IDs to permute but not to communicate.
    ///
    /// The number of IDs that are local to the calling process, but
    /// not part of the first getNumSameIDs() entries.  The Import
    /// will permute these entries locally (without distributed-memory
    /// communication).
    size_t getNumPermuteIDs() const;

    //! List of local IDs in the source Map that are permuted.
    Teuchos::ArrayView<const LocalOrdinal> getPermuteFromLIDs() const;

    //! List of local IDs in the target Map that are permuted.
    Teuchos::ArrayView<const LocalOrdinal> getPermuteToLIDs() const;

    //! Number of entries not on the calling process.
    size_t getNumRemoteIDs() const;

    //! List of entries in the target Map to receive from other processes.
    Teuchos::ArrayView<const LocalOrdinal> getRemoteLIDs() const;

    //! Number of entries that must be sent by the calling process to other processes.
    size_t getNumExportIDs() const;

    //! List of entries in the source Map that will be sent to other processes.
    Teuchos::ArrayView<const LocalOrdinal> getExportLIDs() const;

    /// \brief List of processes to which entries will be sent.
    ///
    /// The entry with Local ID <tt>getExportLIDs()[i]</tt> will be
    /// sent to process <tt>getExportPIDs()[i]</tt>.
    Teuchos::ArrayView<const int> getExportPIDs() const;

    //! The Source Map used to construct this Import object.
    Teuchos::RCP<const map_type> getSourceMap () const;

    //! The Target Map used to construct this Import object.
    Teuchos::RCP<const map_type> getTargetMap () const;

    //! The Distributor that this Import object uses to move data.
    Distributor & getDistributor() const;

    /// \brief Do all target Map indices on the calling process exist
    ///   on at least one process (not necessarily this one) in the
    ///   source Map?
    ///
    /// It's not necessarily an error for an Import not to be locally
    /// complete on one or more processes.  For example, this may
    /// happen in the common use case of "restriction" -- that is,
    /// taking a subset of a large object.  Nevertheless, you may find
    /// this predicate useful for figuring out whether you set up your
    /// Maps in the way that you expect.
    bool isLocallyComplete () const;

    //! Assignment operator.
    Import<LocalOrdinal,GlobalOrdinal,Node>&
    operator= (const Import<LocalOrdinal,GlobalOrdinal,Node>& Source);

    /// \brief Find the union of the target IDs from two Import objects.
    ///
    /// On return, the input arrays permuteGIDs[1,2] and remotePGIDs[1,2] will
    /// be ordered.  unionTgtGIDs are ordered as [{same}, {permute}, {remote}].
    /// {same} is ordered identically to the target map with most "same"
    /// indices.  {permute} is ordered from smallest ID to largest.  {remote} is
    /// ordered by remote process ID and ID, respectively.  remotePGIDs are
    /// ordered the same as {remote}.
    void
    findUnionTargetGIDs(Teuchos::Array<GlobalOrdinal>& unionTgtGIDs,
                        Teuchos::Array<std::pair<int,GlobalOrdinal>>& remotePGIDs,
                        typename Teuchos::Array<GlobalOrdinal>::size_type& numSameGIDs,
                        typename Teuchos::Array<GlobalOrdinal>::size_type& numPermuteGIDs,
                        typename Teuchos::Array<GlobalOrdinal>::size_type& numRemoteGIDs,
                        const Teuchos::ArrayView<const GlobalOrdinal>& sameGIDs1,
                        const Teuchos::ArrayView<const GlobalOrdinal>& sameGIDs2,
                        Teuchos::Array<GlobalOrdinal>& permuteGIDs1,
                        Teuchos::Array<GlobalOrdinal>& permuteGIDs2,
                        Teuchos::Array<GlobalOrdinal>& remoteGIDs1,
                        Teuchos::Array<GlobalOrdinal>& remoteGIDs2,
                        Teuchos::Array<int>& remotePIDs1,
                        Teuchos::Array<int>& remotePIDs2) const;

    /// \brief Return the union of this Import and \c rhs.
    ///
    /// The "union" of two Import objects is the Import whose source
    /// Map is the same as the input Imports' source Maps, and whose
    /// target Map is the union of the two input Imports' target Maps.
    /// The two input Import objects must have the same source Map.
    /// The union operation is symmetric in its two inputs.
    ///
    /// The communicator of \c rhs must be the same as (MPI_EQUAL) or
    /// congruent with (MPI_CONGRUENT) this Import's communicator.
    /// (That is, the two communicators must have the same number of
    /// processes, and each process must have the same rank in both
    /// communicators.)  This method must be called collectively over
    /// that communicator.
    ///
    /// The Map that results from this operation does <i>not</i>
    /// preserve the original order of global indices in either of the
    /// two input Maps.  Instead, it sorts global indices on each
    /// process so that owned indices occur first, in the same order
    /// as in the source Map, and so that remote indices are sorted in
    /// order of their sending process rank.  This makes communication
    /// operations faster.
    ///
    /// This primitive is useful for adding two sparse matrices
    /// (CrsMatrix), since its can skip over many of the steps of
    /// creating the result matrix's column Map from scratch.
    ///
    /// We have to call this method "setUnion" rather than "union,"
    /// because \c union is a reserved keyword in C++ (and C).  It
    /// would also be reasonable to call this <tt>operator+</tt>,
    /// though it would be a bit confusing for operator+ to return a
    /// pointer but take a reference (or to take a pointer, but have
    /// the left-hand side of the + expression be a reference).
    Teuchos::RCP<const Import<LocalOrdinal, GlobalOrdinal, Node> >
    setUnion (const Import<LocalOrdinal, GlobalOrdinal, Node>& rhs) const;

    /// \brief Return the union of this Import this->getSourceMap()
    ///
    /// This special case of setUnion creates a new Import object
    /// such that the targetMap of the new object contains all local
    /// unknowns in the sourceMap (plus whatever remotes were contained
    /// in this->getSourceMap()).
    ///
    /// The Map that results from this operation does <i>not</i>
    /// preserve the input order of global indices.  All local global
    /// indices are ordered in the order of the sourceMap, all remotes
    /// are ordered as implied by the Importer for *this.
    ///
    /// This primitive is useful for adding or multipyling two sparse matrices
    /// (CrsMatrix), since its can skip over many of the steps of
    /// creating the result matrix's column Map from scratch.
    ///
    Teuchos::RCP<const Import<LocalOrdinal, GlobalOrdinal, Node> >
    setUnion () const;


    /// \brief Returns an importer that contains only the remote entries of this
    ///
    /// Returns an importer that contains only the remote entries of this importer.
    /// It is expected that remoteTarget represents such a map.
    Teuchos::RCP<const Import<LocalOrdinal, GlobalOrdinal, Node> >
    createRemoteOnlyImport (const Teuchos::RCP<const map_type>& remoteTarget) const;


    //@}
    //! @name I/O Methods
    //@{

    /// \brief Describe this object in a human-readable way to the
    ///   given output stream.
    ///
    /// You must call this method as a collective over all processes
    /// in the communicator of the source and target Map of this
    /// object.
    ///
    /// \param out [out] Output stream to which to write.  Only
    ///   Process 0 in this object's communicator may write to the
    ///   output stream.
    ///
    /// \param verbLevel [in] Verbosity level.  This also controls
    ///   whether this method does any communication.  At verbosity
    ///   levels higher (greater) than Teuchos::VERB_LOW, this method
    ///   behaves as a collective over the object's communicator.
    ///
    /// Teuchos::FancyOStream wraps std::ostream.  It adds features
    /// like tab levels.  If you just want to wrap std::cout, try
    /// this:
    /// \code
    /// auto out = Teuchos::getFancyOStream (Teuchos::rcpFromRef (std::out));
    /// \endcode
    virtual void
    describe (Teuchos::FancyOStream& out,
              const Teuchos::EVerbosityLevel verbLevel =
                Teuchos::Describable::verbLevel_default) const;

    /// \brief Print the Import's data to the given output stream.
    ///
    /// This method assumes that the given output stream can be
    /// written on all process(es) in the Import's communicator.  The
    /// resulting output is useful mainly for debugging.
    ///
    /// \note This method tries its best (by using barriers at the end
    ///   of each iteration of a for loop over all communicator ranks)
    ///   to ensure ordered deterministic output.  However, the
    ///   assumption that all processes can write to the stream means
    ///   that there are no ordering guarantees other than what the
    ///   operating and run-time system provide.  (MPI synchronization
    ///   may be separate from output stream synchronization, so the
    ///   barriers only improve the chances that output can complete
    ///   before the next process starts writing.)
    virtual void print (std::ostream& os) const;

    //@}
  private:
    //! All the data needed for executing the Import communication plan.
    Teuchos::RCP<ImportExportData<LocalOrdinal,GlobalOrdinal,Node> > ImportData_;
    //! Output stream for debug output.
    Teuchos::RCP<Teuchos::FancyOStream> out_;
    //! Whether to print copious debug output on each process.
    bool debug_;

    //! @name Initialization helper functions (called by the constructor)
    //@{

    /// \brief Initialize the Import.  Called by all constructors.
    ///
    /// \param source [in] The source distribution.  This <i>must</i>
    ///   be a uniquely owned (nonoverlapping) distribution.
    ///
    /// \param target [in] The target distribution.  This may be a
    ///   multiply owned (overlapping) distribution.
    ///
    /// \param useRemotePIDs [in] True if the remotePIDs parameter is non-empty
    /// on at least some processors.
    ///
    /// \param remotePIDs [in/out] Owning PIDs corresponding to the remoteGIDs.
    ///
    /// \param plist [in/out] List of parameters.  Currently passed
    ///   directly to the Distributor that implements communication.
    ///   If this is Teuchos::null, it is not used.
    void
    init (const Teuchos::RCP<const map_type>& source,
          const Teuchos::RCP<const map_type>& target,
          bool useRemotePIDs,
          Teuchos::Array<int> & remotePIDs,
          const Teuchos::RCP<Teuchos::ParameterList>& plist);

    /// \brief Compute the necessary receives for the Import.
    ///
    /// This routine fills in the following fields of ImportData_:
    ///
    ///   - numSameIDs_ (the number of consecutive initial GIDs owned
    ///     by both the source and target Maps)
    ///   - permuteToLIDs_ (for each of the remaining GIDs g in the
    ///     target Map, if the source Map also owns g, then
    ///     permuteToLIDs_ gets the corresponding LID in the target,
    ///     and permuteFromLIDs_ gets the corresponding LID in the
    ///     source)
    ///   - permuteFromLIDs_ (see permuteToLIDs_)
    ///   - remoteLIDs_ (the LIDs of the GIDs that are owned by the
    ///     target Map, but not by the source Map)
    ///
    /// It also allocates and fills in the temporary remoteGIDs array
    /// with the GIDs that are owned by the target Map but not by the
    /// source Map.
    ///
    /// The name for this routine comes from what it does.  It first
    /// finds the GIDs that are the same (representing elements which
    /// require neither communication nor permutation).  Then it finds
    /// permutation IDs (which require permutation, but no
    /// communication, because they are in a possibly different order
    /// in the source and target Maps, but owned by the same process)
    /// and remote IDs (which require communication, because they are
    /// owned by the target Map but not by the source Map).
    ///
    /// This routine does not communicate, except perhaps for the
    /// TPETRA_ABUSE_WARNING (that is only triggered if there are
    /// remote IDs but the source is not distributed).
    void setupSamePermuteRemote (Teuchos::Array<GlobalOrdinal>& remoteGIDs);

    /// \brief Compute the send communication plan from the receives.
    ///
    /// This routine is called after setupSamePermuteRemote(), if the
    /// source Map is distributed.  It uses the <tt>remoteGIDs</tt>
    /// temporary array that was allocated by that routine.  After
    /// this routine completes, the <tt>remoteGIDs</tt> array is no
    /// longer needed.
    ///
    /// The remotePIDs argument is optional.  If the remotePIDs array is of
    /// size zero, then it will be computed via a call to getRemoteIndexList.
    /// If it isn't zero, it must match the initial size of the remoteGIDs.
    ///
    /// Algorithm:
    ///
    /// 1. Identify which GIDs are in the target Map but not in the
    ///    source Map.  These correspond to required receives.  Store
    ///    them for now in <tt>remoteGIDs</tt>.  Find the process IDs
    ///    of the remote GIDs to receive.
    ///
    /// 2. Invoke Distributor's createFromRecvs() using the above
    ///    remote GIDs and remote process IDs as input.  This sets up
    ///    the Distributor and computes the send GIDs and process IDs.
    ///
    /// 3. Use the source Map to compute the send LIDs from the send
    ///    GIDs.
    ///
    /// This routine fills in the <tt>remoteLIDs_</tt> field of
    /// <tt>ImportData_</tt>.
    void
    setupExport (Teuchos::Array<GlobalOrdinal>& remoteGIDs, 
		 bool useRemotePIDs, Teuchos::Array<int> & remotePIDs,
		 const Teuchos::RCP<Teuchos::ParameterList>& plist= Teuchos::null);
    //@}

    /// \brief "Expert" constructor that includes all the Import's data.
    ///
    /// This is useful for implementing setUnion() efficiently.
    /// Arguments passed in as nonconst references (including
    /// Teuchos::Array objects and the Distributor) are invalidated on
    /// exit.  This lets this constructor exploit their swap() methods
    /// so that it doesn't have to copy them.
    Import (const Teuchos::RCP<const map_type>& source,
            const Teuchos::RCP<const map_type>& target,
            const size_t numSameID,
            Teuchos::Array<LocalOrdinal>& permuteToLIDs,
            Teuchos::Array<LocalOrdinal>& permuteFromLIDs,
            Teuchos::Array<LocalOrdinal>& remoteLIDs,
            Teuchos::Array<LocalOrdinal>& exportLIDs,
            Teuchos::Array<int>& exportPIDs,
            Distributor& distributor,
            const Teuchos::RCP<Teuchos::FancyOStream>& out = Teuchos::null,
            const Teuchos::RCP<Teuchos::ParameterList>& plist = Teuchos::null);


  }; // class Import

  /** \brief Nonmember constructor for Import.

      Create a Import object from the given source and target Maps.
      \pre <tt>src != null</tt>
      \pre <tt>tgt != null</tt>
      \return The Import object. If <tt>src == tgt</tt>, returns \c null.
        (Debug mode: throws std::runtime_error if one of \c src or \c tgt is \c null.)

      \relatesalso Import
    */
  template<class LocalOrdinal, class GlobalOrdinal, class Node>
  Teuchos::RCP<const Import<LocalOrdinal, GlobalOrdinal, Node> >
  createImport (const Teuchos::RCP<const Map<LocalOrdinal, GlobalOrdinal, Node> >& src,
                const Teuchos::RCP<const Map<LocalOrdinal, GlobalOrdinal, Node> >& tgt)
  {
    if (src == tgt) {
      return Teuchos::null;
    }
#ifdef HAVE_TPETRA_DEBUG
    TEUCHOS_TEST_FOR_EXCEPTION(
      src == Teuchos::null || tgt == Teuchos::null, std::runtime_error,
      "Tpetra::createImport(): neither source nor target map may be null:"
      << std::endl << "source: " << src << std::endl << "target: " << tgt
      << std::endl);
#endif // HAVE_TPETRA_DEBUG
    typedef Import<LocalOrdinal, GlobalOrdinal, Node> import_type;
    return Teuchos::rcp (new import_type (src, tgt));
  }

  /** \brief Nonmember constructor for Import that takes a ParameterList.

      Create a Import object from the given source and target Maps,
      using the given list of parameters.
      \pre <tt>src != null</tt>
      \pre <tt>tgt != null</tt>
      \return The Import object. If <tt>src == tgt</tt>, returns \c null.
        (Debug mode: throws std::runtime_error if one of \c src or \c tgt is \c null.)

      \relatesalso Import
    */
  template<class LocalOrdinal, class GlobalOrdinal, class Node>
  Teuchos::RCP<const Import<LocalOrdinal, GlobalOrdinal, Node> >
  createImport (const Teuchos::RCP<const Map<LocalOrdinal, GlobalOrdinal, Node> >& src,
                const Teuchos::RCP<const Map<LocalOrdinal, GlobalOrdinal, Node> >& tgt,
                const Teuchos::RCP<Teuchos::ParameterList>& plist)
  {
    if (src == tgt) {
      return Teuchos::null;
    }
#ifdef HAVE_TPETRA_DEBUG
    TEUCHOS_TEST_FOR_EXCEPTION(
      src == Teuchos::null || tgt == Teuchos::null, std::runtime_error,
      "Tpetra::createImport(): neither source nor target map may be null:"
      << std::endl << "source: " << src << std::endl << "target: " << tgt
      << std::endl);
#endif // HAVE_TPETRA_DEBUG
    typedef Import<LocalOrdinal, GlobalOrdinal, Node> import_type;
    return Teuchos::rcp (new import_type (src, tgt, plist));
  }

} // namespace Tpetra

#endif // TPETRA_IMPORT_DECL_HPP
