// -*- C++ -*-
/*! \file
 *  \brief Construct group baryon operators
 */

#ifndef __group_baryon_operator_w_h__
#define __group_baryon_operator_w_h__

#include "handle.h"
#include "meas/hadron/baryon_operator.h"
#include "meas/smear/quark_smearing.h"
#include "io/xml_group_reader.h"
#include "io/qprop_io.h"
#include "io/cfgtype_io.h"
#include "meas/hadron/baryon_operator_aggregate_w.h"
#include "meas/hadron/baryon_operator_factory_w.h"
#include "meas/sources/source_smearing_aggregate.h"
#include "meas/sources/source_smearing_factory.h"
#include "meas/sinks/sink_smearing_aggregate.h"
#include "meas/sinks/sink_smearing_factory.h"
#include "meas/smear/quark_source_sink.h"
#include "meas/sources/dilutezN_source_const.h"
#include "meas/sources/zN_src.h"

namespace Chroma
{
//#define MAKE_SOURCE_OPERATORS
#define MAKE_SINK_OPERATORS
#define PARALLEL_RUN
//#define REDUCETOTIMEDILUTION 1

  //! Name and registration
  /*! @ingroup hadron */
  namespace GroupBaryonOperatorEnv
  {
    bool registerAll();
    //! Group baryon operator parameters
    /*! @ingroup hadron */
	
	  struct BaryonOperator_t
	  {
	    //! Serialize generalized operator object
	    multi1d<Complex> serialize();
	    //! Baryon operator
	    struct BaryonOperatorInsertion_t
	    {
	      //! Possible operator index
	      struct BaryonOperatorIndex_t
	      {
	        //! Baryon operator element
	        struct BaryonOperatorElement_t
	        {
	          multi2d<DComplex> elem; 
	        };
	        multi1d<BaryonOperatorElement_t> ind;
	      };
	      multi3d<BaryonOperatorIndex_t> op; 
	    };
    	Seed seed_l;            /*!< Id of left quark */
    	Seed seed_m;            /*!< Id of middle quark */
    	Seed seed_r;            /*!< Id of right quark */

      multi1d< multi1d<int> > quarkOrderings; // [ index ][ LMR ]=0,1,2
    	int mom2_max;
    	int j_decay;
			int version;
	    multi1d<BaryonOperatorInsertion_t> orderings; 
	  };
		
		int DilSwap( int ord, int i, int j, int k, int which );
		int DilSwapInv( int ord, int i, int j, int k, int which );

    struct Params
    {
      Params();
      Params( XMLReader& in, const std::string& path );
      void writeXML( XMLWriter& in, const std::string& path ) const;
    
			PropSourceSmear_t source_smearing;
    	PropSinkSmear_t sink_smearing;

      struct param_t
			{
				std::string baryon_operator; // NUCLEON, DELTA
			} 
			param;
			
			std::string name; // ?
      int j_decay;  
			
      std::string InputFileName; // Input generated by "Gen_Input_For_All2All_Baryons2.pl"
      multi1d<std::string> Names; // Names of operators ... "G1g_L3_TDT_15"
      multi1d<int> nrow; // lattice size Nx Ny Nz Nt
      int Nmomenta, mom2_max; 
      int Noperators; // total number of baryon operators to be constructed
      int NQQQs;      // total number of QQQ's
      int NsrcOrderings, NsnkOrderings; 
      multi1d< multi1d<int> > SrcOrderings; // [ index ][ LMR ]=0,1,2
      multi1d< multi1d<int> > SnkOrderings; // [ index ][ LMR ]=0,1,2
						
      // the hybrid-list (dilution) sizes
      multi1d< multi1d<int> > NH; // NH[ ord ][ 0,1,2 ] 
    				
			struct Qprop_t // Prop_t
			{
			  struct Solutions_t // Operator_t
			  {
					multi1d<std::string> soln_file_names; // soln_files
					Seed seed;
			  };
			  multi1d<Solutions_t> solns; // quark index (0,1,2)  op
      	std::string op_file;
			} 
			qprop; // prop
			
  		//! Structure holding a source and its solutions
  		struct QuarkSourceSolutions_t
  		{
  		  //! Structure holding solutions
  		  struct QuarkSolution_t
  		  {
  		    LatticeFermion source;
  		    LatticeFermion soln;
  		    PropSourceConst_t source_header;
  		    ChromaProp_t prop_header;
  		  };
  		  int j_decay;
  		  Seed seed;
  		  multi1d<QuarkSolution_t> dilutions;
  		};
			
			struct gaugestuff_t
			{
				Cfg_t cfg;
				multi1d<LatticeColorMatrix> u;
				GroupXML_t link_smearing;
			} 
			gaugestuff;
			
			struct dilution_t
			{
				int N; /*!< Z(N) */
				multi1d<int> spatial_mask_size; /*!< Spatial size of periodic mask */
				multi1d< multi1d<int> > spatial_mask; /*!< Sites included in site mask */
				multi1d<int> color_mask; /*!< Color size of periodic mask */
				multi1d<int> spin_mask; /*!< Spin size of periodic mask */
				int j_decay;  // is 3
				int t_source; // assuming time-dilution
				Seed ran_seed;
			};
			
			multi1d<dilution_t> dilution;
			
			struct NamedObject_t
    	{
    	  QuarkSourceSolutions_t vecs;
				Qprop_t qprop;
				BaryonOperator_t Bops;
    	  std::string gauge_id;
    	} 
			named_obj;						

			int NdilReduce;
			
    }; // end struct Params
		
		
	  //! Structure holding a source and its solutions
	  struct QuarkSourceSolutions_t
	  {
	    //! Structure holding solutions
	    struct QuarkSolution_t
	    {
	      LatticeFermion source;
	      LatticeFermion soln;
	      PropSourceConst_t source_header;
	      ChromaProp_t prop_header;
	    };
	    int j_decay;
	    Seed seed;
	    multi1d<QuarkSolution_t> dilutions;
	  };


    class GroupBaryonOp //: public BaryonOperator<LatticeFermion>
    {
      public:
				GroupBaryonOp()
        {}
				~GroupBaryonOp()
				{}
				//! Baryon operator
	  		BaryonOperator_t baryonoperator;
				
				std::string name;
        Params myparams;
    }; // end class GroupBaryonOp


    /*! @ingroup hadron
     *
     * Create a group theoretical construction baryon
     */
    class GroupBaryonQQQ : public BaryonOperator<LatticeFermion>
    {
      public:
    		void init( const Params& p );
        //! Full constructor
        GroupBaryonQQQ( const Params& p );
        ~GroupBaryonQQQ(){}
				// ?
				std::string name;			

        //! Compute the operator
        multi1d<LatticeComplex> operator() ( const LatticeFermion& quark1,
                                             const LatticeFermion& quark2,
                                             const LatticeFermion& quark3,
																						 int* qindices,
                                             enum PlusMinus isign 
                                           ) const;

        LatticeComplex operator() ( const LatticeFermion& quark1,
                                    const LatticeFermion& quark2,
                                    const LatticeFermion& quark3,
																		int* qindices
                                  ) const;
        //! Compute the operator
        multi1d<LatticeComplex> operator() ( const LatticeFermion& quark1,
                                             const LatticeFermion& quark2,
                                             const LatticeFermion& quark3,
                                             enum PlusMinus isign 
                                           ) const;

        LatticeComplex operator() ( const LatticeFermion& quark1,
                                    const LatticeFermion& quark2,
                                    const LatticeFermion& quark3
                                  ) const;

        struct QuarkTerm_t
        {
          int displacement;    /*!< Orig plus/minus 1-based directional displacements */
          int spin;            /*!< 0-based spin index */
          int disp_dir;        /*!< 0-based direction */
          int disp_len;        /*!< 0-based length */
          int disp_ind;        // disp index 0=0, 1=1, 2=2, 3=3, 4=-1, 5=-2, 6=-3
        };
        multi1d<QuarkTerm_t> quark; /*!< Displacement and spin for each quark */
        
        // Number of Baryon operators requiring this QQQ
        int NBaryonOps;  // this is the size of the arrays below
        multi1d<DComplex> coef;      // coef[ local_BOpIndex ]
        multi1d<int> whichBaryonOps; // whichBaryonOps[local_BopIndex] of size NBaryonOps
        multi1d<std::string> Names;  // and their names
        multi1d<GroupBaryonOp*> baryon; // pointer to the GroupBaryonOp array

        //private: 
        //! Hide partial constructor
        GroupBaryonQQQ();
        Params myparams;
				SpinMatrix spin_rotate_mat;

        //protected:
      	//Handle< QuarkSourceSink<LatticeFermion> > sourceSmearing;
      	//Handle< QuarkSourceSink<LatticeFermion> > sinkSmearing;

        //! Construct array of std::maps of displacements
        void displaceQuarks( multi1d< std::map<int, LatticeFermion> >& disp_quarks,
                             const multi1d<LatticeFermion>& q,
														 int* qindices
                           ) const;
        //! First smear then displace the quarks
        void smearDisplaceQuarks( multi1d< std::map<int, LatticeFermion> >& disp_quarks,
                                  const LatticeFermion& q1,
                                  const LatticeFermion& q2,
                                  const LatticeFermion& q3,
																	int* qindices
                                ) const;
        //! Manipulate the quark fields
        void quarkManip( multi1d< std::map<int, LatticeFermion> >& disp_quarks,
                         const LatticeFermion& q1,
                         const LatticeFermion& q2,
                         const LatticeFermion& q3,
												 int* qindices
                       ) const;
        //! The spin basis matrix to goto Dirac
        const SpinMatrix& rotateMat() const
        {
          return spin_rotate_mat;
        }
				LatticeFermion operator() ( Params::dilution_t ) const;

    }; // end class GroupBaryonQQQ
		
    //
    // read in input parameters from an ASCII file
    //
    void ReadTextInput( Params&, multi1d<GroupBaryonOp>&, multi1d<GroupBaryonOp>&,
                        multi1d<GroupBaryonQQQ>&, multi1d<GroupBaryonQQQ>& );
		
  	//! Reader
  	/*! @ingroup hadron */

  	void read( XMLReader& xml, const std::string& path, 
		           GroupBaryonOperatorEnv::Params::Qprop_t::Solutions_t& input );
  	void read( XMLReader& xml, const std::string& path, 
		           GroupBaryonOperatorEnv::Params::Qprop_t& input );
  	void read( XMLReader& xml, const std::string& path, 
		           GroupBaryonOperatorEnv::Params& input );

  }  // end namespace GroupBaryonOperatorEnv

  //! Writer
  /*! @ingroup hadron */
  void write( XMLWriter& xml, const std::string& path, 
	            const GroupBaryonOperatorEnv::Params& param );

}  // end namespace Chroma

//
// Perl script to generate the input file
//
/*
#! /usr/bin/perl
#------------------------------------------------------------------------
# The output file of this script ... the name is kind of important 
# because the xml input file is not ready to go yet and hence the name 
# will be used to identify the record of the inputs
$OutputFileName = "Chroma_Input.txt"; 
#------------------------------------------------------------------------
# Some other files that will be generated ...
$SortedListFileName = "SortedChannelList.txt";
       $ListOfNames = "NamesOfOperators.txt";
#------------------------------------------------------------------------
$ProjCoeffDir = "$ENV{HOME}/src2/work2/projection_coefficients/Nucleons";
#------------------------------------------------------------------------
# Input file for operator indices (note that the range op - doesn't work)
# an example is given below ...
$OperatorIndicesFile = "from_point2all.in";
#------------------------------------------------------------------------
# Lattice Size
@Nsize = ( 12, 12, 12, 48 ); 
$Nt = $Nsize[ 3 ];
#------------------------------------------------------------------------
# Hybrid list size
for(my $q=1; $q<=3; ++$q) 
{
	$NTdil[$q] = $Nt; $NCdil[$q] = 1; $NSdil[$q] = 1; $NXdil[$q] = 1;
	#$NTdil[$q] = $Nt; $NCdil[$q] = 3; $NSdil[$q] = 4; $NXdil[$q] = 1;
}
for(my $q=1; $q<=3; ++$q){ $NH[$q]=$NTdil[$q]*$NCdil[$q]*$NSdil[$q]*$NXdil[$q]; }
#
@Ndil = ( $NH[1], $NH[2], $NH[3] );
#------------------------------------------------------------------------
# Different displacement lengths
@DispLengths = ( 3 );
# all the channels
@Channels = ( 'G1g', 'G1u', 'G2g', 'G2u', 'Hg', 'Hu' );
#------------------------------------------------------------------------
# Solution file names  arg: stub, time-dil, colour-dil, spin-dil, space-dil, ".lime"
$qstub = "wl_6p1_12_48_m0p305_nu0p902"; $qext = ".lime"; 
@qpropFileNames = ();
for(my $q=1; $q<=3; ++$q)
{
	@temp = &make_qprop_names( "${qstub}${q}",$NTdil[$q],$NCdil[$q],$NSdil[$q],$NXdil[$q],$qext );
	push( @qpropFileNames, @temp );
}
#------------------------------------------------------------------------
# Nucleon correlation function is re-written as,
#
#    (N+)         (I)   ijk               _ijk     _ijk      _ijk               
#   C        =   c     B      (t) {     2 B  ___ - B  ___  - B  ___             
#    IJ[012]      mnt   I<mnt>             J<mnt>   J<mtn>    J<nmt>            
#                                         _ijk     _ijk      _ijk               
#                                   + ( 2 B  ___ - B  ___  - B  ___  )          
#                                          J<tnm>   J<tmn>    J<ntm>            
#                                       _(J) 														           
#                                 }(t0) c___														           
#                                        mnt 														           
#------------------------------------------------------------------------
# Input File Example: (from_point2all.in)
#
#    <G1>
# 	 SS  0  1  2
# 	 SD  0 11 17 20 22
# 	 DDI 0  4  5  9 12
# 	 DDL 1  4 10 15 21
# 	 TDT 3  5  9 11 25
# 	 <G2>
# 	 SS   
# 	 SD   0  1  2  6  7
# 	 DDI  1  4  5  6  7
# 	 DDL 32 37 41 52 61
# 	 TDT  1 33 45 51 61
# 	 <H>
# 	 SS   0
# 	 SD   7  9 10  13  31
# 	 DDI  8 15 17  23  31
# 	 DDL 47 54 84 113 124
# 	 TDT 35 71 86  95 104
#
#------------------------------------------------------------------------
# 					Nothing from here needs to be touched
#------------------------------------------------------------------------
#
# Permutations of noise indices
#
$NumSrcPerm = 6; # number of perm of 0,1,2 for SOURCE
	@srcOrder0 = ( 0, 1, 2 ); $SrcPerm{0} = "srcOrder0";
	@srcOrder1 = ( 2, 1, 0 ); $SrcPerm{1} = "srcOrder1";
	@srcOrder2 = ( 0, 2, 1 ); $SrcPerm{2} = "srcOrder2"; 
	@srcOrder3 = ( 1, 0, 2 ); $SrcPerm{3} = "srcOrder3";
	@srcOrder4 = ( 1, 2, 0 ); $SrcPerm{4} = "srcOrder4";
	@srcOrder5 = ( 2, 0, 1 ); $SrcPerm{5} = "srcOrder5";
$NumSnkPerm = 1; # number of perm of 0,1,2 for SINK
	@snkOrder0 = ( 0, 1, 2 ); $SnkPerm{0} = "snkOrder0";
#------------------------------------------------------------------------
@ListOfIrreps  = ( 'G1', 'G2', 'H' ); # in the input file
@OperatorTypes = ( 'SS', 'SD', 'DDI', 'DDL', 'TDT' );
$LongForm{'SS'}  = "Single_Site";
$LongForm{'SD'}  = "Singly_Displaced";
$LongForm{'DDI'} = "Doubly_Displaced_I";
$LongForm{'DDL'} = "Doubly_Displaced_L";
$LongForm{'TDT'} = "Triply_Displaced_T";
$QNS{'G1g'} = 'G1'; $QNS{'G1u'} = 'G1';
$QNS{'G2g'} = 'G2'; $QNS{'G2u'} = 'G2';
 $QNS{'Hg'} = 'H';   $QNS{'Hu'} = 'H';
#------------------------------------------------------------------------
#
# First get all of the operator indices
#
if ( 1 ) {
  $Noperators = 0;
  foreach $irrep ( @ListOfIrreps ) {
    $name = $irrep.'ops'; # ex. G1ops
    &GetOperatorList( $OperatorIndicesFile, ListOfIrreps, $irrep, $name );
    foreach $type ( @OperatorTypes ) {
      $name2 = $name.'List_'.$type; # ex. G1opsList_SS
      @$name2 = &GetNums( $$name{"$type"} ); # ex. $G1ops{"SS"}
      foreach $channel ( @Channels ) {
				if ( ($QNS{$channel} eq $irrep) && (@$name2 ne {}) ) {
					if ( $type eq 'SS' ) {
						$Noperators += (scalar(@$name2));
					} else {
						$Noperators += (scalar(@$name2)) * (scalar(@DispLengths));
	} } } } }
}
#
# Generate the hash table
#
if ( 1 ) {
  open(HASH,">Hashes");
  foreach my $QN ( @Channels ) 
	{
  	$QNS = $QNS{"$QN"};
	  foreach my $OperatorType ( @OperatorTypes ) 
		{
      $InputFileName = "${ProjCoeffDir}/${QN}/$LongForm{\"$OperatorType\"}";
      $ListName = "${QNS}opsList_${OperatorType}";
      foreach my $OperatorIndx ( @${ListName} ) 
			{
	      $Nterms = 0;
        ( $Nterms, %list ) = &ReadProjCoeffFile( $InputFileName, $OperatorIndx );
	      foreach my $DispLength ( @DispLengths ) 
				{
	        #
	        # SS L1
	        #
	        if ( (${OperatorType}eq'SS')and(${DispLength}==${DispLengths[0]}) ) 
					{
	            for (my $i=0; $i<$Nterms; ++$i) 
							{
                @IJK = &GetNums( $list{$i} );
	              $line = '';
                # DO NOT use the coefficients here
                for (my $j=0; $j<($#IJK-1); ++$j) { $line = $line . "$IJK[$j] "; }
	              $line = $line . "0";
	              #$line = $line . ${DispLength};
	              $hash = &HashCode($line);
	              printf(HASH "${hash}\n");
	            }
	        }
	        #
	        # SD L1 L2 : DDI L1 L2 : DDL L1 L2 : TDT L1 L2
	        #
	        if ( ${OperatorType} ne 'SS' ) 
					{
	            for (my $i=0; $i<$Nterms; ++$i) 
							{
                @IJK = &GetNums( $list{$i} );
	              $line = '';
                # DO NOT use the coefficients here
                for (my $j=0; $j<($#IJK-1); ++$j) { $line = $line . "$IJK[$j] "; }
	              $line = $line . ${DispLength};
	              $hash = &HashCode($line);
	              printf(HASH "${hash}\n");
	            }
	        }
	      } # end foreach $DispLength ( @DispLengths )
	    } # end foreach $OperatorIndx ( @${ListName} )
	  } # end foreach $OperatorType ( @OperatorTypes )
	} # end foreach $QN ( @Channels )
  close(HASH);

	system( "sort Hashes | uniq > Hashes2" );
  open(FILE,"<Hashes2");
	@UniqHashes = ();
	while (<FILE>) { chop($_); push( @UniqHashes, $_ ); }
  close(FILE);
	system("rm Hashes Hashes2");
}
#
# Main Loop over Irreps
#
foreach $QN ( @Channels ) 
{ 
	&MakeOpTable($QN); 
}
#
# Sorted List (according to channel name)
#   "Name re im # s1 s2 s3 d1 d2 d3 dL"
#
if ( 1 ) {
	open(FILE,">$SortedListFileName");
  open(FILENAMES,">$ListOfNames");
  foreach my $QN ( @Channels ) 
	{
    $QNS = $QNS{"$QN"};
    foreach my $OperatorType ( @OperatorTypes ) 
		{
      $ListName = "${QNS}opsList_${OperatorType}";
      $InputFileName = "${ProjCoeffDir}/${QN}/$LongForm{\"$OperatorType\"}";
      foreach my $OperatorIndx ( @${ListName} ) 
			{
        ( $Nterms, %list ) = &ReadProjCoeffFile( $InputFileName, $OperatorIndx );
        for (my $i=0; $i<$Nterms; ++$i) 
				{ 
          @IJK = &GetNums( $list{$i} );
          foreach my $DispLength ( @DispLengths ) 
					{
            $line = '';
            for (my $j=0; $j<($#IJK-1); ++$j) { $line=$line."$IJK[$j] "; }
            if(($OperatorType eq 'SS')&&(${DispLength}==${DispLengths[0]})) 
						{ 
							$line=$line." 0"; 
            } else { 
							$line=$line." ${DispLength}"; 
						}
            $hash = &HashCode($line);
            for (my $r=0; $r <= $#{$IrrepsInHash{$hash}}; $r=$r+3) 
						{
              $newline = "${$IrrepsInHash{$hash}}[$r] ${$IrrepsInHash{$hash}}[$r+1] ${$IrrepsInHash{$hash}}[$r+2] ${line}";
							printf(FILE "$newline\n");
							printf(FILENAMES "${$IrrepsInHash{$hash}}[$r]\n");
  } } } } } }
  close(FILE);
  close(FILENAME);
  system("sort ${SortedListFileName} | uniq > mytemp");
  system("mv mytemp ${SortedListFileName}");
  system("sort ${ListOfNames} | uniq > myDistinctOperators");
	open(FILE,"<myDistinctOperators");
	open(OUTFILE,">DistinctOperators");
  $GlobalNumbering = 0;
	while (<FILE>) {
		chop($_);
		printf(OUTFILE "%-3i %s\n",$GlobalNumbering,$_);
		$OperatorNames{"$_"} = $GlobalNumbering;
		$GlobalNumbering++;
	}
	close(FILE);
	close(OUTFILE);
	system("wc -l DistinctOperators > mytemp2");
	system("cat mytemp2 DistinctOperators > ${ListOfNames}");
	system("rm -f mytemp2 myDistinctOperators DistinctOperators");
}
#
# Output Results to File
#   "# s1 s2 s3 d1 d2 d3 dL NtermsNeedingThisComb"
#   "index" corresponding to "Name" (check NamesOfOperators.txt)
#   "re im "
#
if ( 1 ) {
  open(FILE,">${OutputFileName}");
	printf(FILE "${Nsize[0]} ${Nsize[1]} ${Nsize[2]} ${Nsize[3]}\n");
	printf(FILE "$NumSrcPerm\n");
	for (my $i=0; $i < $NumSrcPerm; ++$i)
	{
		printf(FILE "${$SrcPerm{$i}}[0] ${$SrcPerm{$i}}[1] ${$SrcPerm{$i}}[2]\n");
	}
	# added so that XML and TXT input files are more similar
	printf(FILE "$NumSnkPerm\n");
	for (my $i=0; $i < $NumSnkPerm; ++$i)
	{
		printf(FILE "${$SnkPerm{$i}}[0] ${$SnkPerm{$i}}[1] ${$SnkPerm{$i}}[2]\n");
	}
	printf(FILE "${Ndil[0]} ${Ndil[1]} ${Ndil[2]}\n");
	printf(FILE "${Noperators}\n");
	# moved here so that XML and TXT input files are more similar
  	open(INFILE,"<${ListOfNames}");
		$line = <INFILE>;
		while ( <INFILE> ) 
		{
			printf(FILE "$_");
		}
		close(INFILE);
	#
	$NElemOps = scalar(@UniqHashes);
	printf(FILE "${NElemOps}\n");
	foreach $hash ( @UniqHashes ) 
	{
		$line = $InvHashCode{$hash}; # all 7 indices
		@indices = &GetNums( $line );
		printf(FILE "${hash} ");
		PrintList2FILE( FILE, @indices );
		my $N = ( scalar(@{$IrrepsInHash{$hash}}) ) / 3;
		printf(FILE "${N}\n"); # number of ops which need this qqq
		for (my $r=0; $r <= $#{$IrrepsInHash{$hash}}; $r=$r+3) 
		{
		 #printf(FILE                "${$IrrepsInHash{$hash}}[$r]\n");
			printf(FILE "$OperatorNames{${$IrrepsInHash{$hash}}[$r]}\n");
			printf(FILE "${$IrrepsInHash{$hash}}[$r+1]");
			printf(FILE " ${$IrrepsInHash{$hash}}[$r+2]\n");
		}
	}
	for (my $i=0; $i <= $#{qpropFileNames}; ++$i) 
	{
		printf(FILE "$qpropFileNames[$i]\n");
	}
	close(FILE);
}

#===================================================================================

# name s1 s2 s3 d1 d2 d3  h
#   0   1  2  3  4  5  6  7  (use with "sort list_by_hash_value keys %hasharray")
sub list_by_hash_value { ($shash{$a}[7]) <=> ($shash{$b}[7]); };
sub PrintList{ foreach $i (@_) { printf("%2s ",$i); } print "\n";};
sub PrintList2FILEN
{ local *HANDLE = shift;
	foreach$i(@_){printf(HANDLE "%2s ",$i);}printf(HANDLE "\n");
}
sub PrintList2FILE 
{ local *HANDLE = shift;
	foreach$i(@_){printf(HANDLE "%2s ",$i);}
}
sub HashCode
{ my ( $line ) = @_;
  @in = &GetNums(@_); # s1 s2 s3 d1 d2 d3 dL
	$hash_code = ( ( $in[0] - 1 ) << 17 ) # spins from 1
             + ( ( $in[1] - 1 ) << 15 )
             + ( ( $in[2] - 1 ) << 13 )
             + ( ( $in[3] + 3 ) << 10 ) # displ dirs from -3
             + ( ( $in[4] + 3 ) << 7 )
             + ( ( $in[5] + 3 ) << 4 )
             + (   $in[6] );            # disp length index from 1
	$InvHashCode{$hash_code} = "$in[0] $in[1] $in[2] $in[3] $in[4] $in[5] $in[6]";
	$GetHashCode{$line} = $hash_code;
	return($hash_code);
}
sub MakeOpTable
{ # Coefficient File: ${ProjCoeffDir}/${QN}/$LongForm{\"$OperatorType\"}
	my($irrep,$type,$name,$name2,$IndexName,$ListName,$InputFileName);
	my($Nterms,$line,$hash);
	$QN = $_[0];
	$QNS = $QNS{"$QN"};
	print "$QNS $QN\n";
  foreach my $DispLength ( @DispLengths ) {
    foreach my $OperatorType ( @OperatorTypes ) {
      $InputFileName = "${ProjCoeffDir}/${QN}/$LongForm{\"$OperatorType\"}";
      $ListName = "${QNS}opsList_${OperatorType}";
			if ( ${OperatorType} eq 'SS' ) {
				if ( ${DispLength} == ${DispLengths[0]} ) {
			    foreach my $OperatorIndx ( @${ListName} ) {
            $Nterms = 0;
			      ( $Nterms, %list ) = &ReadProjCoeffFile($InputFileName, $OperatorIndx);
            $IndexName = "${OperatorType}_${OperatorIndx}";
            for (my $i=0; $i<$Nterms; ++$i) { 
              @IJK = &GetNums( $list{$i} );
              $line = '';
              for (my $j=0; $j<($#IJK-1); ++$j) { 
                $line = $line . "$IJK[$j] ";
              }
			        $line = $line . "0";
			        #$line = $line . ${DispLength};
              $hash = &HashCode( $line );
							push( @{$IrrepsInHash{$hash}}, "${QN}_L0_${IndexName}" );
							#push( @{$IrrepsInHash{$hash}}, "${QN}_L${DispLength}_${IndexName}" );
							push( @{$IrrepsInHash{$hash}}, $IJK[($#IJK-1)] );
							push( @{$IrrepsInHash{$hash}}, $IJK[($#IJK)] );
            } # end of going thru terms
			    } # end foreach OperatorIndx
				} # end if (${DispLength} == ${DispLengths[0]})
			} # end if SS 
			if ( ${OperatorType} ne 'SS' ) {
			  foreach my $OperatorIndx ( @${ListName} ) {
          $Nterms = 0;
			    ( $Nterms, %list ) = &ReadProjCoeffFile($InputFileName, $OperatorIndx);
          $IndexName = "${OperatorType}_${OperatorIndx}";
          for (my $i=0; $i<$Nterms; ++$i) { 
            @IJK = &GetNums( $list{$i} );
            $line = '';
            for (my $j=0; $j<($#IJK-1); ++$j) { 
              $line = $line . "$IJK[$j] ";
            }
            $line = $line . "$DispLength";
            $hash = &HashCode( $line );
						push( @{$IrrepsInHash{$hash}}, "${QN}_L${DispLength}_${IndexName}" );
						push( @{$IrrepsInHash{$hash}}, $IJK[($#IJK-1)] );
						push( @{$IrrepsInHash{$hash}}, $IJK[($#IJK)] );
          } # end of $i<Nterms loop
			  } # end of : foreach OperatorIndx
			} # end of : ne SS
    } # end OperatorType loop
  } # end DispLength loop
};
sub ReadProjCoeffFile 
{ my ( $FileName, $TargetIndx ) = @_;
	my($NumOps,$OpIndx,$Nterms_,$NTerms,$line,$k,$len);
  open( PROJCOEF, "< $FileName" );
  $NumOps = <PROJCOEF>; chop($NumOps); $NumOps*=1;
  $OpIndx = -1; $Nterms_ = 0; $NTerms = 0; %mylist = ();
  while( <PROJCOEF> )
  { chomp;
    $len = split;
    if( $len == 1 ) {
      ( $Nterms_ ) = split; $OpIndx++;
    } else {
      if( $OpIndx == $TargetIndx ) {
        $line = $_;
        my @entry = &GetNums($line);
				$mylist{"0"}="$entry[0] $entry[1] $entry[2] $entry[3] $entry[4] $entry[5] $entry[6] $entry[7]";
        for($k=1; $k < $Nterms_; ++$k) {
          $line = <PROJCOEF>; chomp;
          @entry = &GetNums($line);
					$mylist{"$k"}="$entry[0] $entry[1] $entry[2] $entry[3] $entry[4] $entry[5] $entry[6] $entry[7]";
        }
				$NTerms = $Nterms_;
			}
		}
  } close( PROJCOEF );
	return( $NTerms, %mylist );
};
sub GetOperatorList
{ my ( $filename, $list, $channel, $outputref ) = @_;
	my ( $irrep, $line, $oplist );
	open(FILE,"<$filename");
	foreach $irrep ( @{$list} ) {
  	$line = <FILE>; # irrep
    $line = <FILE>; chop($line);
		if ( ($irrep eq "$channel") and ( $line ) ) {
			${$outputref}{'SS'} = $line; }
    $line = <FILE>; 
		if ( ($irrep eq "$channel") and ( $line ) ) {
			${$outputref}{'SD'} = $line; }
    $line = <FILE>; 
		if ( ($irrep eq "$channel") and ( $line ) ) {
    	${$outputref}{'DDI'} = $line; }
    $line = <FILE>; 
		if ( ($irrep eq "$channel") and ( $line ) ) {
    	${$outputref}{'DDL'} = $line; }
    $line = <FILE>; 
		if ( ($irrep eq "$channel") and ( $line ) ) {
    	${$outputref}{'TDT'} = $line; }
	} close(FILE);
};
sub RmTrail 
{ my($i,$j,$len,$c,$Out);
	$i=0; $len=length($_[0]); $j=$len;
	while($i<$len){ $c=substr($_[0],$j-1,1);
	if($c =~ /[.0-9]/){$Out=substr($_[0],0,$len-$i);$i=$len;}$j--;$i++;}
	$_[0]=$Out;
};
sub GetNums # ASCII separated by space, comma, semi-colon, colon
{ my ( $line ) = @_; # can simplify this routine for ints only ...
  my($k,$char,$i,$j,$num,$prev);
	$k=0;$i=0;$j=0;$num='';$char='a';@ReturnThis=();# $j is counter
	while(($i<=length($line))and($char ne "\n")and($char ne "")){
		$char=substr($line,$i,1);if(($char eq ',')or($char eq ';')or
		($char eq ':')){if(length($num)>0){&RmTrail($num);
		push(@ReturnThis,$num);}$num='';$j++;}else{if((($char eq '+')
		or($char eq '-'))and(length($num)==0)){$num=$char;$k++;}else
		{if($k>0){if($char=~/[0-9,.,E,e,D,d,\-,+]/){
		$prev=substr($num,$k-1,1);if(not(($char eq '0')and(
		($prev eq '+')or($prev eq '-')))){if(($char eq 'D')or($prev eq 'd')
		){$num=$num.'E';$k++;}else{$num=$num.$char;$k++;}}}else{if(
		length($num)>0){&RmTrail($num);push(@ReturnThis,$num);$num='';
		$j++;}}}else{if($char=~/[0-9,.,\-,+]/){$num=$num.$char;$k++;}}}}
	$i++;}
	return(@ReturnThis);
};
sub sign
{ my ( $line ) = @_;
  my $sign_ = 1;
	@spin = &GetNums(@_); # s1 s2 s3
	$count = 0;
  for (my $i=0; $i < 3; ++$i) {
		if ( $spin[ $i ] >= 3 ) { $count++; }
	}
	$odd = ( $count%2 );
	if ( $odd ) {	$sign_ = -1; }
	return( $sign_ );
}
sub make_qprop_names 
{ my ( $stub, $TD, $CD, $SD, $XD, $ext ) = @_;
	# TfSeCfGf
	if( ($CD >= 2)&&($SD >= 2)&&($XD >= 2) )
	{ my $i = 0;
		for(my $t=0; $t<$TD; ++$t) {
		for(my $c=0; $c<$CD; ++$c) {
		for(my $s=0; $s<$SD; ++$s) {
		for(my $x=0; $x<$XD; ++$x) {
			$FileNames[ $i ] = $stub . "_t${t}_c${c}_s${s}_x${x}" . $ext;
			$i++; }}}}}
	# TfSxCfGf
	if( ($CD >= 2)&&($SD >= 2)&&($XD == 1) )
	{ my $i = 0;
		for(my $t=0; $t<$TD; ++$t) {
		for(my $c=0; $c<$CD; ++$c) {
		for(my $s=0; $s<$SD; ++$s) {
			$FileNames[ $i ] = $stub . "_t${t}_c${c}_s${s}" . $ext;
			$i++; }}}}
	# TfSeCfGx
	if( ($CD >= 2)&&($SD == 1)&&($XD >= 2) )
	{ my $i = 0;
		for(my $t=0; $t<$TD; ++$t) {
		for(my $c=0; $c<$CD; ++$c) {
		for(my $x=0; $x<$XD; ++$x) {
			$FileNames[ $i ] = $stub . "_t${t}_c${c}_x${x}" . $ext;
			$i++; }}}}
	# TfSeCxGf
	if( ($CD == 1)&&($SD >= 2)&&($XD >= 2) )
	{ my $i = 0;
		for(my $t=0; $t<$TD; ++$t) {
		for(my $s=0; $s<$SD; ++$s) {
		for(my $x=0; $x<$XD; ++$x) {
			$FileNames[ $i ] = $stub . "_t${t}_s${s}_x${x}" . $ext;
			$i++; }}}}
	# TfSeCxGx
	if( ($CD == 1)&&($SD == 1)&&($XD >= 2) )
	{ my $i = 0;
		for(my $t=0; $t<$TD; ++$t) {
		for(my $x=0; $x<$XD; ++$x) {
			$FileNames[ $i ] = $stub . "_t${t}_x${x}" . $ext;
			$i++; }}}
	# TfSxCfGx
	if( ($CD >= 2)&&($SD == 1)&&($XD == 1) )
	{ my $i = 0;
		for(my $t=0; $t<$TD; ++$t) {
		for(my $c=0; $c<$CD; ++$c) {
			$FileNames[ $i ] = $stub . "_t${t}_c${c}" . $ext;
			$i++; }}}
	# TfSxCxGf
	if( ($CD == 1)&&($SD >= 2)&&($XD == 1) )
	{ my $i = 0;
		for(my $t=0; $t<$TD; ++$t) {
		for(my $s=0; $s<$SD; ++$s) {
			$FileNames[ $i ] = $stub . "_t${t}_s${s}" . $ext;
			$i++; }}}
	# TfSxCxGx
	if( ($CD == 1)&&($SD == 1)&&($XD == 1) )
	{ my $i = 0;
		for(my $t=0; $t<$TD; ++$t) {
			$FileNames[ $i ] = $stub . "_t${t}" . $ext;
			$i++; }}
	return( @FileNames );
}

1;
# end
*/
#endif