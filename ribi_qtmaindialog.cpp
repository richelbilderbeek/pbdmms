#include "ribi_qtmaindialog.h"

#include <cassert>
#include <stdexcept>

#include "ribi_results.h"
#include "ribi_simulation.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "ui_ribi_qtmaindialog.h"
#pragma GCC diagnostic pop

ribi::qtmaindialog::qtmaindialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ribi_qtmaindialog)
{
  ui->setupUi(this);
  on_button_clicked();
}

ribi::qtmaindialog::~qtmaindialog()
{
  delete ui;
}

ribi::parameters ribi::qtmaindialog::create_parameters() const
{
  const int max_genetic_distance{
    std::stoi(
      ui->parameters->item(0,0)->text().toStdString()
    )
  };
  const int n_generations{
    std::stoi(
      ui->parameters->item(1,0)->text().toStdString()
    )
  };
  const int n_pin_loci{
    std::stoi(
      ui->parameters->item(2,0)->text().toStdString()
    )
  };
  const int n_sil_loci{
    std::stoi(
      ui->parameters->item(3,0)->text().toStdString()
    )
  };
  const double pin_mutation_rate{
    std::stod(
      ui->parameters->item(4,0)->text().toStdString()
    )
  };
  const int population_size{
    std::stoi(
      ui->parameters->item(5,0)->text().toStdString()
    )
  };
  const std::string& rgfgraph_filename{
    ui->parameters->item(6,0)->text().toStdString()
  };
  const int rng_seed{
    std::stoi(
      ui->parameters->item(7,0)->text().toStdString()
    )
  };
  const int sampling_interval{
    std::stoi(
      ui->parameters->item(8,0)->text().toStdString()
    )
  };
  const double sil_mutation_rate{
    std::stod(
      ui->parameters->item(9,0)->text().toStdString()
    )
  };

  return parameters(
    max_genetic_distance,
    n_generations,
    static_cast<std::size_t>(n_pin_loci),
    static_cast<std::size_t>(n_sil_loci),
    pin_mutation_rate,
    population_size,
    rgfgraph_filename, //results_genotype_frequency_graph_filename
    rng_seed,
    sampling_interval,
    sil_mutation_rate
  );
}

void ribi::qtmaindialog::on_button_clicked()
{
  try
  {
    const auto p = create_parameters();
    simulation s(p);
    s.run();
    results r = s.get_results();
    r.save_all(p.get_filename_genotype_frequency_graph());
    const std::string png_filename{
      get_filename_png(p.get_filename_genotype_frequency_graph())
    };
    QPixmap pixmap;
    pixmap.load(png_filename.c_str());
    ui->result->setPixmap(pixmap);
  }
  catch (std::invalid_argument& e)
  {
    ui->result->setText(e.what());
  }
}
